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
    void setCamera(const std::string& name, const ShaderCamera& camera);
    void setLight(const std::string& name, const ShaderLight& light);
    void setParallelLight(const std::string& name, const ShaderParallelLight& light);

    void enable();
    bool checkVertice(const std::string& name);
    unsigned int getVerticeSlotId(const std::string& name);

public:
    static ShaderProgram& getRectShaderProg();
    static ShaderProgram& getCuboidShaderProg();
    static ShaderProgram& getLightSourceShaderProg();
    static std::map<ShaderProgram*, int>& getAllShaderProg();

private:
    static unsigned int BuildShader(const char* shaderCode, unsigned int shaderType);

private:
    unsigned int _progId;

    std::map<std::string, int> _attrNameMap;

    static std::map<ShaderProgram*, int> _registProgramMap;
};

std::string GetCurPath();

#endif // #_HEAD_FLAG_SHADER_H
