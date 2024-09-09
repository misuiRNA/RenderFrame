#ifndef _HEAD_FLAG_SHADER_H
#define _HEAD_FLAG_SHADER_H

#include <string>
#include <vector>
#include <map>
#include "BaseDefine.h"
#include "ShaderCamera.h"
#include "ShaderLight.h"


struct ShaderProgram {
    ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr);
    ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr, const std::string& gsShaderCodeStr);
    ShaderProgram& operator =(const ShaderProgram& oth) = delete;

    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, float v1, float v2, float v3);
    void setUniform(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniform(const std::string& name, const Matrix4X4& material);
    void setUniform(const std::string& name, const XYZ& value);
    void setUniform(const std::string& name, const Color& color);
    void setUniform(const std::string& name, const ShaderLight& light);
    void setUniform(const std::string& name, const ShaderCamera& camera);

    void setCamera(const std::string& name, const ShaderCamera& camera);
    void setLight(const std::string& name, const ShaderLight& light);

    void enable();

    template<typename T>
    void SetUniformList(const std::string& name, const std::vector<T>& items) {
        for (int i = 0; i < items.size(); ++i) {
            std::string itemName = name + UniformArraySuffix(i);
            setUniform(itemName, items[i]);
        }
    }

public:
    static std::string UniformArraySuffix(int index);
    static std::map<ShaderProgram*, int>& GetAllShaderProg();

private:
    static unsigned int BuildShader(const char* shaderCode, unsigned int shaderType);

private:
    unsigned int _progId;

private:
    static std::map<ShaderProgram*, int> _registProgramMap;
};

#endif // #_HEAD_FLAG_SHADER_H
