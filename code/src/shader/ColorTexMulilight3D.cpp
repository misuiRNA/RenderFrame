#include "shader/ColorTexMulilight3D.h"
#include "ShaderProgram.h"
#include "Utils.h"


const ShaderAttribDescriptor ColorTexMulilight3DVertex::DESCRIPTOR = DESC_NEW(ColorTexMulilight3DVertex,
    ITEM(ColorTexMulilight3DVertex, 0, "aPos",      pos),
    ITEM(ColorTexMulilight3DVertex, 1, "aTexCoord", texCoord),
    ITEM(ColorTexMulilight3DVertex, 2, "aNormal",   normal)
);

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorTexMulilight3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorTexMulilight3DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


ColorTexMulilight3D::ColorTexMulilight3D(const Size3D& size)
: AbstractShader(GetShaderProg())
, _attitudeCtrl({0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, size)
, _imageEnable(false)
, _color(1.0f, 1.0f, 1.0f) {

    _renderData.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
}

ColorTexMulilight3D::ColorTexMulilight3D(const ColorTexMulilight3D& oth)
: AbstractShader(GetShaderProg())
, _attitudeCtrl(oth._attitudeCtrl)
, _imageEnable(oth._imageEnable)
, _color(oth._color) {

    _renderData.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
}

void ColorTexMulilight3D::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void ColorTexMulilight3D::setSize(const Size3D& size) {
    _attitudeCtrl.setSize(size);
}

void ColorTexMulilight3D::setColor(const Color& color) {
    _color = color;
}

void ColorTexMulilight3D::setPrimaryImage(const AbstractImage& image) {
    _renderData.setTexture("texture1", image.getTexture(ImageWrapMode::Repeat));
    _imageEnable = true;
}

void ColorTexMulilight3D::setSecondaryImage(const AbstractImage& image) {
    _renderData.setTexture("texture2", image.getTexture(ImageWrapMode::Repeat));
    _imageEnable = true;
}

Attitude3DController& ColorTexMulilight3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void ColorTexMulilight3D::setMaterial(const ShaderMaterial& material) {
    _renderData.setUniform("material", material);
}

void ColorTexMulilight3D::updateUniformes() {
    _renderData.setUniform("imageEnable", _imageEnable);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _renderData.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}
