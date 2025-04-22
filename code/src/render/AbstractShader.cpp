#include "render/AbstractShader.h"


AbstractShader::AbstractShader(ShaderProgram& prog)
: _engine(prog) {

}

AbstractShader::~AbstractShader() {

}

void AbstractShader::show() {
    updateUniformes();
    _engine.draw();
}

void AbstractShader::setDrawMode(RenderDataMode mode) {
    _engine.setDrawMode(mode);
}

void AbstractShader::setVertexData(size_t vertexCount, const void* vvertexData, const ShaderAttribDescriptor& desc, const std::vector<unsigned int>& indices) {
    if (vvertexData == nullptr) {
        return;
    }
    // TODO: 优化, 重新设置顶点数据后需要清除旧顶点VBO等
    _engine.setVertices(vertexCount, vvertexData, desc);
    if (!indices.empty()) {
        _engine.setIndices(indices);
    }
}
