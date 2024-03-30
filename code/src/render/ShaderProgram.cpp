#include <iostream>
#include <fstream>
#include <sstream>
#include "ShaderProgram.h"
#include "glad/glad.h"
#include <unistd.h>

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

void ShaderProgram::setUniform(const std::string& name, const XYZ& value) {
    setUniform(name, value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string& name, const Color& color) {
    setUniform(name,  color.r, color.g, color.b);
}

void ShaderProgram::setCamera(const std::string& name, const ShaderCamera& camera) {
    setUniformMat4(name + ".matrix", camera.getMatrix());
    setUniform(name + ".pos", camera.getPosition());
}

void ShaderProgram::setLight(const std::string& name, const ShaderLight& light) {
    // glsl 传输结构体uniform格式如 light.pos
    setUniform(name + ".pos", light.getPosition());
    setUniform(name + ".ambient", light.getAmbientColor());
    setUniform(name + ".diffuse", light.getDiffuseColor());
    setUniform(name + ".specular", light.getSpecularColor());
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

static std::string ReadFile(const std::string& path) {
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

ShaderProgram& ShaderProgram::getRectShaderProg() {
    static const std::string MODEL_NAME = "Rectangle";
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/RectangleShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/RectangleShader.fs");
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
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Cuboid.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Cuboid.fs");
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
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/LightSource.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/LightSource.fs");
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
