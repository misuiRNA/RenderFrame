#include "model/Model3D.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Model3DlShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Model3DlShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


Model3D::Model3D()
: AbstractDrawObject(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}) {

}

void Model3D::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void Model3D::setSize(const Size3D& size) {
    _attitudeCtrl.setSize(size);
}

void Model3D::setTexture(const std::string& name, unsigned int textureId) {
    _renderData.setTexture(name, textureId);
}

void Model3D::setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",       0, Model3D::Vertex, pos),
        DESC("aNormal",    1, Model3D::Vertex, normal),
        DESC("aTexCoords", 2, Model3D::Vertex, texCoords),
    };

    _renderData.setVertices(vertices, descriptor);

    if (!indices.empty()) {
        _renderData.setIndices(indices);
    }
}

Attitude3DController& Model3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void Model3D::updateUniformes() {
    _renderData.setUniform("model", _attitudeCtrl.getMatrix());
}
