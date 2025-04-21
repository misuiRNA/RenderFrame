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
        _engine.setVertices(vertexCount, vvertexData, desc);
        if (!indices.empty()) {
            _engine.setIndices(indices);
        }
    }

    void setVertexData(const RenderShape& shape) {
        setVertexData(shape.getVertexSize(), shape.getVertexData(), shape.getVertexDescriptor(), shape.indices);
    }

private:
    virtual void updateUniformes() = 0;

protected:
    RenderEngine _engine;
};

#endif
