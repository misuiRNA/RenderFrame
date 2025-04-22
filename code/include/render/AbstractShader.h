#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderEngine.h"
#include "base/Utils.h"


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

    // TODO: 优化, 重新设置顶点数据后需要清除旧顶点VBO等
    void setVertexData(size_t vertexCount, const void* vvertexData, const ShaderAttribDescriptor& desc, const std::vector<unsigned int>& indices) {
        if (vvertexData == nullptr) {
            return;
        }
        _engine.setVertices(vertexCount, vvertexData, desc);
        if (!indices.empty()) {
            _engine.setIndices(indices);
        }
    }

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
