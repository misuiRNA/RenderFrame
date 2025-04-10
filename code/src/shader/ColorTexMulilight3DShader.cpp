#include "shader/ColorTexMulilight3DShader.h"
#include "ShaderProgram.h"
#include "Utils.h"


const ShaderAttribDescriptor ColorTexMulilight3DVertex::DESCRIPTOR = DESC_NEW(ColorTexMulilight3DVertex,
    ITEM(ColorTexMulilight3DVertex, 0, "aPos",      pos),
    ITEM(ColorTexMulilight3DVertex, 1, "aTexCoord", texCoord),
    ITEM(ColorTexMulilight3DVertex, 2, "aNormal",   normal)
);

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorTexMulilight3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorTexMulilight3DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


ColorTexMulilight3DShader::ColorTexMulilight3DShader(const Size3D& size)
: AbstractShader(GetShaderProg())
, _attitudeCtrl({0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, size)
, _imageEnable(false)
, _color(1.0f, 1.0f, 1.0f) {

    _engine.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
}

ColorTexMulilight3DShader::ColorTexMulilight3DShader(const ColorTexMulilight3DShader& oth)
: AbstractShader(GetShaderProg())
, _attitudeCtrl(oth._attitudeCtrl)
, _imageEnable(oth._imageEnable)
, _color(oth._color) {
    new (&_engine)RenderEngine(oth._engine);    // 强制使用 RenderEngine 拷贝构造, 保证 _engine 可用
    _engine.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
}

void ColorTexMulilight3DShader::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void ColorTexMulilight3DShader::setSize(const Size3D& size) {
    _attitudeCtrl.setSize(size);
}

void ColorTexMulilight3DShader::setColor(const Color& color) {
    _color = color;
}

void ColorTexMulilight3DShader::setPrimaryImage(const AbstractImage& image) {
    _engine.setTexture("texture1", image.getTexture(ImageWrapMode::Repeat));
    _imageEnable = true;
}

void ColorTexMulilight3DShader::setSecondaryImage(const AbstractImage& image) {
    _engine.setTexture("texture2", image.getTexture(ImageWrapMode::Repeat));
    _imageEnable = true;
}

Attitude3DController& ColorTexMulilight3DShader::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void ColorTexMulilight3DShader::setMaterial(const ShaderMaterial& material) {
    _engine.setUniform("material", material);
}

void ColorTexMulilight3DShader::updateUniformes() {
    _engine.setUniform("imageEnable", _imageEnable);
    _engine.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _engine.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}
