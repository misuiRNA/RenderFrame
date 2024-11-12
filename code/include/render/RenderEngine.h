#ifndef _HEAD_FLAG_RENDERDATA_H
#define _HEAD_FLAG_RENDERDATA_H

#include <vector>
#include <map>
#include <string>
#include <functional>
#include "BaseDefine.h"
#include "ShaderProgram.h"
#include "ShaderMaterial.h"
#include "Texture.h"
#include <shared_mutex>
#include <iostream>


struct ShaderAttribDescriptor {
    struct AttribItem {
        AttribItem(std::string name, unsigned int index, unsigned int size, const void* data)
        : name(name)
        , index(index)
        , size(size)
        , data(data) { }

        std::string name;
        unsigned int index;
        unsigned int size;
        const void* data;
    };

    ShaderAttribDescriptor(unsigned int stride, const std::vector<AttribItem>& items)
    : stride(stride)
    , items(items) { }

    unsigned int stride;
    std::vector<AttribItem> items;
};


#define DESC_NEW(TYPE, ...) {sizeof(TYPE), {__VA_ARGS__}}

// remind: 要求MEMBER是float紧密填充的, 否则计算出的size不准
#define ITEM(TYPE, INDEX, NAME, MEMBER) {NAME, INDEX, sizeof(TYPE::MEMBER) / sizeof(float), (void*)offsetof(TYPE, MEMBER)}


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

    RenderShape() {}
    RenderShape(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {}) : vertices(vertices), indices(indices) { }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};


struct RenderEngine {
    RenderEngine(ShaderProgram& prog);
    RenderEngine(const RenderEngine& oth);
    RenderEngine(RenderEngine&& oth) noexcept;    // remind: 声明为 noexcept 系统才会优先使用移动构造函数
    ~RenderEngine();
    RenderEngine& operator=(RenderEngine&& oth) = delete;

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
    void setTexture(const std::string& name, TextureId textureId);
    void setDrawMode(RenderDataMode mode);

    ShaderProgram& getShaderProgram() const;
    void draw();
    void setVertices(size_t vertexCount, const void* data, const ShaderAttribDescriptor& desc);
    void setInstanceVertices(size_t vertexCount, const void* data, const ShaderAttribDescriptor& desc);

private:
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
    unsigned int _mode;
    std::shared_ptr<unsigned int> _VAOHolder;

    int _vertexCount;
    int _indexCount;

    int _instanceCount;

    std::map<std::string, TextureId> _textureMap;
    std::map<std::string, std::function<void(ShaderProgram& prog)>> _uniformFunctions;
};

#endif // _HEAD_FLAG_RENDERDATA_H
