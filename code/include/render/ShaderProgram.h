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
    ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr, const std::map<std::string, int>& attrNameMap);

    // TODO 根据需要重载setUniform函数
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, float v1, float v2, float v3);
    void setUniform(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniformMat4(const std::string& name, const float* mat);
    void setUniform(const std::string& name, const XYZ& value);
    void setUniform(const std::string& name, const Color& color);
    void setUniform(const std::string& name, const ShaderLight& light);
    void setUniform(const std::string& name, const ShaderCamera& camera);

    template<typename T>
    void SetUniformList(const std::string& name, const std::vector<T>& items) {
        for (int i = 0; i < items.size(); ++i) {
            std::string itemName = name + UniformArraySuffix(i);
            setUniform(itemName, items[i]);
        }
    }

    void setCamera(const std::string& name, const ShaderCamera& camera);
    void setLight(const std::string& name, const ShaderLight& light);

    void enable();
    bool checkVertice(const std::string& name);
    unsigned int getVerticeSlotId(const std::string& name);

public:
    static std::string UniformArraySuffix(int index);
    static ShaderProgram& GetRectShaderProg();
    static ShaderProgram& GetRect2DShaderProg();
    static ShaderProgram& GetCuboidShaderProg();
    static ShaderProgram& GetLightSourceShaderProg();
    static ShaderProgram& GetMeshShaderProg();
    static std::map<ShaderProgram*, int>& GetAllShaderProg();

private:
    static unsigned int BuildShader(const char* shaderCode, unsigned int shaderType);

private:
    unsigned int _progId;

    std::map<std::string, int> _attrNameMap;

    static std::map<ShaderProgram*, int> _registProgramMap;
};

#endif // #_HEAD_FLAG_SHADER_H
