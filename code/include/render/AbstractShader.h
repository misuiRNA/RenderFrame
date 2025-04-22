#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderEngine.h"
#include "base/Utils.h"


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


struct AbstractShader {
    AbstractShader(ShaderProgram& prog);
    virtual ~AbstractShader();
    void show();
    void setDrawMode(RenderDataMode mode);
    void setVertexData(size_t vertexCount, const void* vvertexData, const ShaderAttribDescriptor& desc, const std::vector<unsigned int>& indices);

    template<typename T>
    void setVertexData(const RenderShapeAdapter<T>& shape) {
        setVertexData(shape.getVertex().size(), shape.getVertex().data(), T::DESCRIPTOR, shape.getIndices());
    }

private:
    virtual void updateUniformes() = 0;

protected:
    RenderEngine _engine;
};

#endif
