#include "shader/Model3D.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/Model3DlShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/Model3DlShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}

const std::vector<ShaderAttribDescriptor> Model3DVertex::DESCRIPTOR = {
    DESC("aPos",       0, Model3DVertex, pos),
    DESC("aNormal",    1, Model3DVertex, normal),
    DESC("aTexCoords", 2, Model3DVertex, texCoords),
};


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
