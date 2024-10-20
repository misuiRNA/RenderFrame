#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderData.h"
#include "Utils.h"

template <typename VertexType>
struct AbstractShader {
    AbstractShader(ShaderProgram& prog) : _renderData(prog) { }
    virtual ~AbstractShader() { }

    void show() {
        updateUniformes();
        _renderData.draw();
    }

    void setDrawMode(RenderDataMode mode) {
        _renderData.setDrawMode(mode);
    }

    // TODO: 优化, 重新设置顶点数据后需要清除旧顶点VBO
    void setVertexData(const std::vector<VertexType>& vertices, const std::vector<unsigned int>& indices) {
        _renderData.setVertices(vertices.size(), vertices.data(), VertexType::DESCRIPTOR);
        if (!indices.empty()) {
            _renderData.setIndices(indices);
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
    RenderData _renderData;
};

#endif
