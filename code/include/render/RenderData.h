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
    ShaderAttribDescriptor(unsigned int index, unsigned int size, unsigned int stride, const void* pointer) : index(index), size(size), stride(stride), pointer(pointer) { }

    unsigned int index;
    unsigned int size;
    unsigned int stride;
    const void* pointer;
};


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
    void setVertices(unsigned int VBO, const std::vector<ShaderAttribDescriptor>& descs);
    void useTextures();
    void resetTextures();
    void useUniforms();
    void drawAttributes();
    void setUniformFunc(const std::string& name, const std::function<void(ShaderProgram& prog)>& func);

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
        unsigned int index = _prog.getVerticeSlotId(name);

        unsigned int size = sizeof(T) / sizeof(float);    // TODO: 优化, T不一定完全是float组成的
        unsigned int stride = sizeof(T);
        ShaderAttribDescriptor desc(index, size, stride, (void*)0);

        unsigned int VBO = CreateVBO(vertices.size() * sizeof(T), vertices.data());
        setVertices(VBO, {desc});
        _vertexCount = vertices.size();
    }

    template <typename T>
    void setVertices(const std::vector<T>& vertices) {
        unsigned int VBO = CreateVBO(vertices.size() * sizeof(T), vertices.data());
        setVertices(VBO, T::descriptor);
        _vertexCount = vertices.size();
    }


private:
    ShaderProgram& _prog;
    unsigned int _VAOId;
    std::shared_ptr<unsigned int> _VAOHolder;

    int _vertexCount;
    int _indexCount;

    std::map<std::string, int> _textureMap;
    std::map<std::string, std::function<void(ShaderProgram& prog)>> _uniformFunctions;
};

#endif // _HEAD_FLAG_RENDERDATA_H
