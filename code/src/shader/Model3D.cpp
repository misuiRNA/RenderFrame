#include "shader/Model3D.h"
#include "Utils.h"


const ShaderAttribDescriptor Model3DVertex::DESCRIPTOR = DESC_NEW(Model3DVertex,
    ITEM(Model3DVertex, 0, "aPos",       pos),
    ITEM(Model3DVertex, 1, "aNormal",    normal),
    ITEM(Model3DVertex, 2, "aTexCoords", texCoords)
);

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/Model3DlShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/Model3DlShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


Model3D::Model3D()
: AbstractShader(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}) {

}

void Model3D::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void Model3D::setSize(const Size3D& size) {
    _attitudeCtrl.setSize(size);
}

void Model3D::setDiffuseImage(int index, const AbstractImage& image) {
    std::string name = "diffuseTexture" + ShaderProgram::UniformArraySuffix(index);
    _renderData.setTexture(name, image.getTexture(ImageWrapMode::Repeat));
}

void Model3D::setSpecularImage(int index, const AbstractImage& image) {
    std::string name = "specularTexture" + ShaderProgram::UniformArraySuffix(index);
    _renderData.setTexture(name, image.getTexture(ImageWrapMode::Repeat));
}

void Model3D::setNormalImage(int index, const AbstractImage& image) {
    std::string name = "normalTexture" + ShaderProgram::UniformArraySuffix(index);
    _renderData.setTexture(name, image.getTexture(ImageWrapMode::Repeat));
}

void Model3D::setHeightImage(int index, const AbstractImage& image) {
    std::string name = "heightTexture" + ShaderProgram::UniformArraySuffix(index);
    _renderData.setTexture(name, image.getTexture(ImageWrapMode::Repeat));
}

Attitude3DController& Model3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void Model3D::updateUniformes() {
    _renderData.setUniform("model", _attitudeCtrl.getMatrix());
}
