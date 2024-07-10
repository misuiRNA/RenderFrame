#ifndef _HEAD_FLAG_SHADER_H
#define _HEAD_FLAG_SHADER_H

#include <string>
#include <vector>
#include <map>
#include "BaseDefine.h"
#include "ShaderCamera.h"
#include "ShaderLight.h"

struct ShaderAttribDescriptor {
    ShaderAttribDescriptor(std::string name, unsigned int index, unsigned int size, unsigned int stride, const void* pointer)
    : name(name)
    , index(index)
    , size(size)
    , stride(stride)
    , pointer(pointer) { }

    std::string name;
    unsigned int index;
    unsigned int size;
    unsigned int stride;
    const void* pointer;
};

// remind: 要求MEMBER是float紧密填充的, 否则计算出的size不准
#define DESC(NAME, INDEX, TYPE, MEMBER) ShaderAttribDescriptor(NAME, INDEX, sizeof(TYPE::MEMBER) / sizeof(float), sizeof(TYPE), (void*)offsetof(TYPE, MEMBER))


struct ShaderProgram {
    ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr, const std::vector<ShaderAttribDescriptor>& descriptors);
    ShaderProgram& operator =(const ShaderProgram& oth) = delete;

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
    unsigned int getVerticeSlotId(const std::string& name);    // TODO: delete
    const std::vector<ShaderAttribDescriptor>& getVertexDescriptors() const;

public:
    static std::string UniformArraySuffix(int index);
    static std::map<ShaderProgram*, int>& GetAllShaderProg();

private:
    static unsigned int BuildShader(const char* shaderCode, unsigned int shaderType);

private:
    unsigned int _progId;
    std::vector<ShaderAttribDescriptor> _vertexDescriptors;

private:
    static std::map<ShaderProgram*, int> _registProgramMap;
};

#endif // #_HEAD_FLAG_SHADER_H
