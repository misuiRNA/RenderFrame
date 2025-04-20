#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderEngine.h"
#include "base/Utils.h"

template <typename VertexType>
struct AbstractShader {
    AbstractShader(ShaderProgram& prog) : _engine(prog) { }
    virtual ~AbstractShader() { }

    void show() {
        updateUniformes();
        _engine.draw();
    }

    void setDrawMode(RenderDataMode mode) {
        _engine.setDrawMode(mode);
    }

    // TODO: 优化, 重新设置顶点数据后需要清除旧顶点VBO
    void setVertexData(const std::vector<VertexType>& vertices, const std::vector<unsigned int>& indices) {
        _engine.setVertices(vertices.size(), vertices.data(), VertexType::DESCRIPTOR);
        if (!indices.empty()) {
            _engine.setIndices(indices);
        }
    }

    void setVertexData(const RenderShape& shape) {
        // TODO: 优化性能, 减少内存拷贝
        std::function<VertexType(const RenderShape::Vertex&)> convert = [](const RenderShape::Vertex& sv) -> VertexType { return {sv}; };
        std::vector<VertexType> vertices = ConvertList(shape.vertices, convert);
        setVertexData(vertices, shape.indices);
    }

private:
    virtual void updateUniformes() = 0;

protected:
    RenderEngine _engine;
};

#endif
