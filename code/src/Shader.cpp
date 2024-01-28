#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <unistd.h>

RenderData::RenderData() : RenderData({}, {}) {

}

RenderData::RenderData(const std::map<std::string, int>& attrNameMap, const std::map<std::string, int>& textureSlotNameMap)
: _vertexCount(0)
, _indexCount(0)
, _attrNameMap(attrNameMap)
, _textureSlotNameMap(textureSlotNameMap) {
    glGenVertexArrays(1, &_VAOId);
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
    setVertices(_attrNameMap[name], vertexSize, vertices);
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
    setTexture(_textureSlotNameMap[name], width, height, imageData, format);
}

void RenderData::draw() {
    glBindVertexArray(_VAOId);
    for (auto& entity : _textureMap) {
        int slotIndex = entity.first;
        unsigned int textureId = entity.second;
        glActiveTexture(GL_TEXTURE0 + slotIndex);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
    if (_indexCount > 0) {
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
    }
}


ShaderProgram::ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr)
: ShaderProgram(vsShaderCodeStr, fsShaderCodeStr, {}) {

}

ShaderProgram::ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr, const std::map<std::string, int>& textureSlotNameMap)
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

void ShaderProgram::setUniform(const std::string& name, float v1, float v2, float v3, float v4) {
    int uniformLocation = glGetUniformLocation(_progId, name.c_str());
    glUseProgram(_progId);
    glUniform4f(uniformLocation, v1, v2, v3, v4);
}

void ShaderProgram::setUniform(const std::string& name, unsigned int size, const float* mat) {
    int uniformLocation = glGetUniformLocation(_progId, name.c_str());
    glUseProgram(_progId);
    switch (size) {
        case 2:
            glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, mat);
            break;
        case 3:
            glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, mat);
            break;
        case 4:
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, mat);
            break;
        default:
            break;
    }
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
