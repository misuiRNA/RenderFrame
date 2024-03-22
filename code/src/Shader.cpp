#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <unistd.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

RenderData::RenderData() : RenderData({}, {}) {

}

RenderData::RenderData(const std::map<std::string, int>& attrNameMap, const std::map<std::string, int>& textureSlotNameMap)
: _vertexCount(0)
, _indexCount(0)
, _attrNameMap(attrNameMap)
, _textureSlotNameMap(textureSlotNameMap) {
    glGenVertexArrays(1, &_VAOId);
    // 保证未设置纹理的 RenderData 不会使用其他 RenderData 的纹理
    for (auto& pair : _textureSlotNameMap) {
        _textureMap[pair.second] = 0;
    }
}

RenderData::~RenderData() {
    glDeleteVertexArrays(1, &_VAOId);
}

void RenderData::setVertices(unsigned int index, unsigned int vertexSize, const std::vector<float>& vertices) {
    glBindVertexArray(_VAOId);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(index, vertexSize, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(index);

    // TODO should release VBO manual ?
    _vertexCount = vertices.size();
}

void RenderData::setVertices(const std::string& name, unsigned int vertexSize, const std::vector<float>& vertices) {
    if (_attrNameMap.find(name) == _attrNameMap.end()) {
        std::cout << "Failed to set attribute! name not found: " << name << std::endl;
        return;
    }
    setVertices(_attrNameMap.at(name), vertexSize, vertices);
}

void RenderData::setIndices(const std::vector<unsigned int>& indices) {
    glBindVertexArray(_VAOId);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // TODO should release EBO manual ?
    _indexCount = indices.size();
}

void RenderData::setTexture(unsigned int slotIndex, unsigned int width, unsigned int height, const unsigned char* imageData, unsigned int format) {
    if (!imageData) {
        std::cout << "Failed to set texture! imageData is null " << std::endl;
        return;
    }

    glActiveTexture(GL_TEXTURE0 + slotIndex);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    _textureMap[slotIndex] = texture;
}

void RenderData::setTexture(const std::string& name, unsigned int width, unsigned int height, const unsigned char* imageData, unsigned int format) {
    if (_textureSlotNameMap.find(name) == _textureSlotNameMap.end()) {
        std::cout << "Failed to set texture! name not found: " << name << std::endl;
        return;
    }
    setTexture(_textureSlotNameMap.at(name), width, height, imageData, format);
}

void RenderData::useTextures() {
    for (auto& entity : _textureMap) {
        int slotIndex = entity.first;
        unsigned int textureId = entity.second;
        glActiveTexture(GL_TEXTURE0 + slotIndex);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
}

void RenderData::draw() {
    glBindVertexArray(_VAOId);
    if (_indexCount > 0) {
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
    }
}

std::map<ShaderProgram*, int> ShaderProgram::_registProgramMap;

ShaderProgram::ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr)
: ShaderProgram(vsShaderCodeStr, fsShaderCodeStr, {}, {}) {

}

ShaderProgram::ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr, const std::map<std::string, int>& attrNameMap, const std::map<std::string, int>& textureSlotNameMap)
: _progId(0)
, _attrNameMap(attrNameMap) 
, _textureSlotNameMap(textureSlotNameMap)
{
    unsigned int vertexShader = BuildShader(vsShaderCodeStr.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = BuildShader(fsShaderCodeStr.c_str(), GL_FRAGMENT_SHADER);

    // TODO check shader compile status

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "[ERROR] LINK_FAILED:\n" << infoLog << std::endl;
    }
    // 删除 shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _progId = shaderProgram;

    for (auto& pair : textureSlotNameMap) {
        setUniform(pair.first, pair.second);
    }

    _registProgramMap[this] += 1;
}

void ShaderProgram::setUniform(const std::string& name, int value) {
    int uniformLocation = glGetUniformLocation(_progId, name.c_str());
    glUseProgram(_progId);
    glUniform1i(uniformLocation, value);
}

void ShaderProgram::setUniform(const std::string& name, float value) {
    int uniformLocation = glGetUniformLocation(_progId, name.c_str());
    glUseProgram(_progId);
    glUniform1f(uniformLocation, value);
}

void ShaderProgram::setUniform(const std::string& name, float v1, float v2, float v3) {
    int uniformLocation = glGetUniformLocation(_progId, name.c_str());
    glUseProgram(_progId);
    glUniform3f(uniformLocation, v1, v2, v3);
}

void ShaderProgram::setUniform(const std::string& name, float v1, float v2, float v3, float v4) {
    int uniformLocation = glGetUniformLocation(_progId, name.c_str());
    glUseProgram(_progId);
    glUniform4f(uniformLocation, v1, v2, v3, v4);
}

void ShaderProgram::setUniformMat4(const std::string& name, const float* mat) {
    int uniformLocation = glGetUniformLocation(_progId, name.c_str());
    glUseProgram(_progId);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, mat);
}

RenderData ShaderProgram::getRenderData() const {
    return RenderData(_attrNameMap, _textureSlotNameMap);
}

void ShaderProgram::draw(RenderData& attribute) {
    glUseProgram(_progId);
    attribute.draw();
}

unsigned int ShaderProgram::BuildShader(const char* shaderCode, unsigned int shaderType) {
    unsigned int shderId = glCreateShader(shaderType);
    glShaderSource(shderId, 1, &shaderCode, NULL);
    glCompileShader(shderId);

    int  success;
    char infoLog[512];
    glGetShaderiv(shderId, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        std::string shaderTypeName = "Invalid Shader Type";
        if (shaderType == GL_VERTEX_SHADER) {
            shaderTypeName = "Vertex";
        } else if (shaderType == GL_FRAGMENT_SHADER) {
            shaderTypeName = "Fragment";
        }
        glGetShaderInfoLog(shderId, 512, NULL, infoLog);
        std::cout << "[ERROR] COMPILATION_FAILED ("<<  shaderTypeName << ") :\n" << infoLog << std::endl;
        std::cout << shaderCode << std::endl;
    }
    return shderId;
}

ShaderProgram& ShaderProgram::getRectShaderProg() {
    static const std::string MODEL_NAME = "Rectangle";
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/RectangleShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/RectangleShader.fs");
    static const std::map<std::string, int> ATTRIBUTE_NAME_MAP ={
        {"aPos"     , 0},
        {"aTexCoord", 1},
    };
    static const std::map<std::string, int> TEXTURE_SLOT_NAME_MAP = {
        {"texture1", 0},
    };

    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, ATTRIBUTE_NAME_MAP, TEXTURE_SLOT_NAME_MAP);
    return prog;
}

ShaderProgram& ShaderProgram::getCuboidShaderProg() {
    static const std::string MODEL_NAME = "Cuboid";
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/Cuboid.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/Cuboid.fs");
    static const std::map<std::string, int> ATTRIBUTE_NAME_MAP ={
        {"aPos"     , 0},
        {"aTexCoord", 1},
        {"aNormal"  , 2},
    };
    // TODO: 优化, 删除 TEXTURE_SLOT_NAME_MAP, 直接在setTexture中设置
    static const std::map<std::string, int> TEXTURE_SLOT_NAME_MAP = {
        {"texture1", 0},
        {"texture2", 1},
    };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, ATTRIBUTE_NAME_MAP, TEXTURE_SLOT_NAME_MAP);
    return prog;
}

ShaderProgram& ShaderProgram::getLightSourceShaderProg() {
    static const std::string MODEL_NAME = "LightSource";
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/LightSource.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/LightSource.fs");
    static const std::map<std::string, int> ATTRIBUTE_NAME_MAP ={
        {"aPos"     , 0},
    };

    static const std::map<std::string, int> TEXTURE_SLOT_NAME_MAP = { };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, ATTRIBUTE_NAME_MAP, TEXTURE_SLOT_NAME_MAP);
    return prog;
}

std::map<ShaderProgram*, int>& ShaderProgram::getAllShaderProg()
{
    return _registProgramMap;
}

// TODO: 优化, 1.shader字符串编译时确定，不读取文件；2.返回的路径位置应为可执行文件位置，而不是执行命令的位置 考虑使用 std::filesystem
std::string GetCurPath() {
    std::string path;
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        path = buffer;
    } else {
        std::cerr << "get cur path error!" << std::endl;
    }
    return path;
}

std::string ReadFile(const std::string& path) {
    std::string path1 = GetCurPath();
    std::string content;
    std::ifstream fileStream;
    fileStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try  {
        fileStream.open(path.c_str());
        std::stringstream contentStream;
        contentStream << fileStream.rdbuf();
        fileStream.close();
        content = contentStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
    }
    return content;
}
