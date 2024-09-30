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
#define DESC(NAME, INDEX, TYPE, MEMBER) ShaderAttribDescriptor(NAME, INDEX, sizeof(TYPE::MEMBER) / sizeof(float), sizeof(TYPE), (void*)offsetof(TYPE, MEMBER))

enum class RenderDataMode {
    POINTS          = 0,
    LINES           = 1,
    LINE_LOOP       = 2,
    LINE_STRIP      = 3,
    TRIANGLES       = 4,
    TRIANGLE_STRIP  = 5,
    TRIANGLE_FAN    = 6
};

struct RenderShape {
    struct Vertex {
        Position pos;
        Vector3D normal;
        Vector2D texcoord;
        Color color;
    };

    RenderShape(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {}) : vertices(vertices), indices(indices) { }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};


struct RenderData {
    RenderData(ShaderProgram& prog, RenderDataMode mode);
    RenderData(const RenderData& oth);
    RenderData(RenderData&& oth) noexcept;    // remind: 声明为 noexcept 系统才会优先使用移动构造函数
    ~RenderData();
    RenderData& operator=(RenderData&& oth) = delete;

    // TODO 根据需要重载setUniform函数
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, float v1, float v2, float v3);
    void setUniform(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniform(const std::string& name, const XYZ& value);
    void setUniform(const std::string& name, const Color& color);
    void setUniform(const std::string& name, const ShaderMaterial& material);
    void setUniform(const std::string& name, const Matrix4X4& material);
    void setIndices(const std::vector<unsigned int>& indices);
    void setTexture(const std::string& name, unsigned int textureId);

    ShaderProgram& getShaderProgram() const;
    void draw();

    template <typename T>
    void setVertices(const std::vector<T>& vertices, const std::vector<ShaderAttribDescriptor>& descs) {
        setVertices(vertices.size(), sizeof(T), vertices.data(), descs);
    }

    template <typename T>
    void setInstanceVertices(const std::vector<T>& vertices, const std::vector<ShaderAttribDescriptor>& descs) {
        setInstanceVertices(vertices.size(), sizeof(T), vertices.data(), descs);
    }

private:
    void setVertices(size_t vertexCount, size_t verticeStride, const void* data, const std::vector<ShaderAttribDescriptor>& descs);
    void setInstanceVertices(size_t vertexCount, size_t verticeStride, const void* data, const std::vector<ShaderAttribDescriptor>& descs);
    void useTextures();
    void resetTextures();
    void useUniforms();
    void setUniformFunc(const std::string& name, const std::function<void(ShaderProgram& prog)>& func);

    unsigned int VAOID() const { return *_VAOHolder; }
    bool isElementDrawMode() const { return _indexCount > 0; }
    bool isInstanceDrawMode() const { return _instanceCount > 0; }

private:
    static unsigned int CreateVBO(size_t size, const void* data);
    static unsigned int CreateEBO(const std::vector<unsigned int>& indices);

private:
    ShaderProgram& _prog;
    const unsigned int _mode;
    std::shared_ptr<unsigned int> _VAOHolder;

    int _vertexCount;
    int _indexCount;

    int _instanceCount;

    std::map<std::string, int> _textureMap;
    std::map<std::string, std::function<void(ShaderProgram& prog)>> _uniformFunctions;
};

#endif // _HEAD_FLAG_RENDERDATA_H
