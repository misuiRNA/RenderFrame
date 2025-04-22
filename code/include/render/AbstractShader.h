#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderEngine.h"
#include "base/Utils.h"


template<typename T>
struct RenderShapeAdapter {
    RenderShapeAdapter(const RenderShape& data)
    :  _vertices(ConvertList(data.vertices, convert))
    // :  _vertices(std::vector<T>(data.vertices.begin(), data.vertices.end()))
    , _indices(data.indices) {}

    const std::vector<T>& getVertex() const { return _vertices; }
    const std::vector<unsigned int> getIndices() const { return _indices; }
    const ShaderAttribDescriptor& getVertexDescriptor() const { return T::DESCRIPTOR; }

private:
    const std::function<T(const RenderShape::Vertex&)> convert = [](const RenderShape::Vertex& sv) -> T { return {sv}; };
    std::vector<T> _vertices;
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
        setVertexData(shape.getVertex().size(), shape.getVertex().data(), shape.getVertexDescriptor(), shape.getIndices());
    }

private:
    virtual void updateUniformes() = 0;

protected:
    RenderEngine _engine;
};

#endif
