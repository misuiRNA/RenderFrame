#ifndef _HEAD_FLAG_RENDERDATA_H
#define _HEAD_FLAG_RENDERDATA_H

#include <vector>
#include <map>
#include <string>
#include <functional>
#include "BaseDefine.h"
#include "ShaderProgram.h"
#include "ShaderMaterial.h"
#include <shared_mutex>
#include <iostream>


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
#define DESC(NAME, INDEX, TYPE, MEMBER) ShaderAttribDescriptor(NAME, INDEX, sizeof(MEMBER) / sizeof(float), sizeof(TYPE), (void*)offsetof(TYPE, MEMBER))



struct RenderData {
    RenderData(ShaderProgram& prog);
    RenderData(const RenderData& oth);
    RenderData(RenderData&& oth) noexcept;    // remind: 声明为 noexcept 系统才会优先使用移动构造函数
    ~RenderData();

    RenderData& operator=(RenderData&& oth) noexcept;

    void setIndices(const std::vector<unsigned int>& indices);
    void setTexture(const std::string& name, unsigned int textureId);

    // TODO 根据需要重载setUniform函数
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, float v1, float v2, float v3);
    void setUniform(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniform(const std::string& name, const XYZ& value);
    void setUniform(const std::string& name, const Color& color);
    void setUniform(const std::string& name, const ShaderMaterial& material);
    void setUniformMat4(const std::string& name, const float* mat);

    ShaderProgram& getShaderProgram() const;

    void draw();

private:
    void setVertices(size_t vertexCount, size_t verticeStride, const void* data, const std::vector<ShaderAttribDescriptor>& descs);
    void useTextures();
    void resetTextures();
    void useUniforms();
    void drawAttributes();
    void setUniformFunc(const std::string& name, const std::function<void(ShaderProgram& prog)>& func);

    unsigned int VAOID() const { return *_VAOHolder; }

private:
    static unsigned int CreateVBO(size_t size, const void* data);
    static unsigned int CreateEBO(const std::vector<unsigned int>& indices);

public:

    template<typename T>
    void setVertices(const std::string& name, const std::vector<T>& vertices) {
        if (!_prog.checkVertice(name)) {
            std::cout << "Failed to set attribute! name not found: " << name << std::endl;
            return;
        }

        // TODO: 优化 ShaderAttribDescriptor.size 计算方式, T不一定完全是float组成的
        std::vector<ShaderAttribDescriptor> descs = {{name, _prog.getVerticeSlotId(name), sizeof(T) / sizeof(float), sizeof(T), (void*)0}};
        setVertices(vertices.size(), sizeof(T), vertices.data(), descs);
    }

    template <typename T>
    void setVertices(const std::vector<T>& vertices) {
        const std::vector<ShaderAttribDescriptor>& descs = T::descriptor;
        setVertices(vertices.size(), sizeof(T), vertices.data(), descs);
    }


private:
    ShaderProgram& _prog;
    std::shared_ptr<unsigned int> _VAOHolder;

    int _vertexCount;
    int _indexCount;

    std::map<std::string, int> _textureMap;
    std::map<std::string, std::function<void(ShaderProgram& prog)>> _uniformFunctions;
};

#endif // _HEAD_FLAG_RENDERDATA_H
