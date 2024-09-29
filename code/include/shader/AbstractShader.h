#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderData.h"

template <typename VertexType>
struct AbstractShader {
    AbstractShader(ShaderProgram& prog, RenderDataMode mode) : _renderData(prog, mode) { }
    virtual ~AbstractShader() { }

    void show() {
        updateUniformes();
        _renderData.draw();
    }

    // TODO: 优化, 重新设置顶点数据后需要清除旧顶点VBO
    void setVertexData(const std::vector<VertexType>& vertices, const std::vector<unsigned int>& indices) {
        _renderData.setVertices(vertices, VertexType::DESCRIPTOR);
        if (!indices.empty()) {
            _renderData.setIndices(indices);
        }
    }

    void setVertexData(const RenderShape& shape) {
        std::vector<VertexType> vertices = shape.getVertices<VertexType>();

        _renderData.setVertices(vertices, VertexType::DESCRIPTOR);
        if (!shape.indices.empty()) {
            _renderData.setIndices(shape.indices);
        }
    }

private:
    virtual void updateUniformes() = 0;

protected:
    RenderData _renderData;
};

#endif
