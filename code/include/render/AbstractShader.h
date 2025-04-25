#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include <vector>
#include <map>
#include <string>
#include "base/BaseDefine.h"
#include "ShaderProgram.h"
#include "ShaderMaterial.h"
#include "Texture.h"
#include "base/Utils.h"


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


template<typename T>
struct RenderShapeAdapter {
    using VertexType = typename T::Vertex;
    using ConvertFunc = std::function<VertexType(const RenderShape::Vertex&)>;

    RenderShapeAdapter(const RenderShape& data)
    :  _vertices(ConvertList(data.vertices, (ConvertFunc)T::BuildVertex))
    , _indices(data.indices) {}

    const std::vector<VertexType>& getVertex() const { return _vertices; }
    const std::vector<unsigned int> getIndices() const { return _indices; }

private:
    std::vector<VertexType> _vertices;
    std::vector<unsigned int> _indices;
};


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

    ShaderAttribDescriptor() {}
    ShaderAttribDescriptor(unsigned int stride, const std::vector<AttribItem>& items)
    : stride(stride)
    , items(items) { }

    unsigned int stride;
    std::vector<AttribItem> items;
};

#define DESC_NEW(TYPE, ...) {sizeof(TYPE), {__VA_ARGS__}}
// remind: 要求MEMBER是float紧密填充的, 否则计算出的size不准
#define ITEM(TYPE, INDEX, NAME, MEMBER) {NAME, INDEX, sizeof(TYPE::MEMBER) / sizeof(float), (void*)offsetof(TYPE, MEMBER)}


struct AbstractShader {
    AbstractShader(ShaderProgram& prog);
    AbstractShader(const AbstractShader& oth);
    AbstractShader(AbstractShader&& oth) noexcept;    // remind: 声明为 noexcept 系统才会优先使用移动构造函数
    virtual ~AbstractShader();
    void show();
    void setDrawMode(RenderDataMode mode);
    void setVertexData(size_t vertexCount, const void* vvertexData, const ShaderAttribDescriptor& desc, const std::vector<unsigned int>& indices);

    template<typename T>
    void setVertexData(const RenderShapeAdapter<T>& shape) {
        setVertexData(shape.getVertex().size(), shape.getVertex().data(), T::DESCRIPTOR, shape.getIndices());
    }

protected:
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
    ShaderProgram& getShaderProgram() const;
    void draw();
    void setVertices(size_t vertexCount, const void* data, const ShaderAttribDescriptor& desc);
    void setInstanceVertices(size_t vertexCount, const void* data, const ShaderAttribDescriptor& desc);

private:
    virtual void updateUniformes() = 0;
    void useTextures();
    void resetTextures();
    void useUniforms();
    void setUniformFunc(const std::string& name, const std::function<void(ShaderProgram& prog)>& func);
    unsigned int VAOID() const { return *_VAOHolder; }
    bool isElementDrawMode() const { return _indexCount > 0; }
    bool isInstanceDrawMode() const { return _instanceCount > 0; }

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

#endif
