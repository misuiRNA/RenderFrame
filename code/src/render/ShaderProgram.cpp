#include <iostream>
#include "ShaderProgram.h"
#include "glad/glad.h"
#include "base/Utils.h"


std::map<ShaderProgram*, int> ShaderProgram::_registProgramMap;

ShaderProgram::ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr)
: ShaderProgram(vsShaderCodeStr, fsShaderCodeStr, "") {

}

ShaderProgram::ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr, const std::string& gsShaderCodeStr)
: _progId(0) {
    bool geometryShaderEnable = !gsShaderCodeStr.empty();

    unsigned int vertexShader = BuildShader(vsShaderCodeStr.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = BuildShader(fsShaderCodeStr.c_str(), GL_FRAGMENT_SHADER);
    unsigned int geometryShader = 0;
    if (geometryShaderEnable) {
        geometryShader = BuildShader(gsShaderCodeStr.c_str(), GL_GEOMETRY_SHADER);
    }

    // TODO check shader compile status

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    if (geometryShaderEnable) {
        glAttachShader(shaderProgram, geometryShader);
    }
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

void ShaderProgram::setUniform(const std::string& name, const Matrix4X4& mat) {
    int uniformLocation = glGetUniformLocation(_progId, name.c_str());
    glUseProgram(_progId);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (const float*)&mat);
}

void ShaderProgram::setUniform(const std::string& name, const XYZ& value) {
    setUniform(name, value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string& name, const Color& color) {
    setUniform(name,  color.r, color.g, color.b);
}

void ShaderProgram::setUniform(const std::string& name, const ShaderCamera& camera) {
    setUniform(name + ".matrix", camera.getMatrix());
    setUniform(name + ".pos", camera.getPosition());
}

void ShaderProgram::setCamera(const std::string& name, const ShaderCamera& camera) {
    setUniform(name, camera);
}

// TODO 优化, 使用组合模式封装 unigform 配置自定义结构体的复杂性, 设计思路可参考 nlohmann json
void ShaderProgram::setUniform(const std::string& name, const ShaderLight& light) {
    // glsl 传输结构体uniform格式如 light.pos
    if (light.isParallel()) {
        setUniform(name + ".direction", light.getSpotDirection());
    } else {
        setUniform(name + ".pos", light.getPosition());
        setUniform(name + ".attenuationKC", light.getAttenuationFactor().x);
        setUniform(name + ".attenuationKL", light.getAttenuationFactor().y);
        setUniform(name + ".attenuationKQ", light.getAttenuationFactor().z);
        setUniform(name + ".spotDirection", light.getSpotDirection());
        setUniform(name + ".spotCos",  MathUtils::AngleCos(light.getSpotAngle()));
        setUniform(name + ".spotOuterCos", MathUtils::AngleCos(light.getSpotOuterAngle()));
    }
    setUniform(name + ".ambient", light.getAmbientColor());
    setUniform(name + ".diffuse", light.getDiffuseColor());
    setUniform(name + ".specular", light.getSpecularColor());
}

void ShaderProgram::setLight(const std::string& name, const ShaderLight& light) {
    setUniform(name, light);
}

void ShaderProgram::enable() {
    glUseProgram(_progId);
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
        } else if (shaderType == GL_GEOMETRY_SHADER) {
            shaderTypeName = "Geometry";
        }
        glGetShaderInfoLog(shderId, 512, NULL, infoLog);
        std::cout << "[ERROR] COMPILATION_FAILED ("<<  shaderTypeName << ") :\n" << infoLog << std::endl;
        std::cout << shaderCode << std::endl;
    }
    return shderId;
}

std::string ShaderProgram::UniformArraySuffix(int index) {
    return "[" + std::to_string(index) + "]";
}

std::map<ShaderProgram*, int>& ShaderProgram::GetAllShaderProg()
{
    return _registProgramMap;
}
