#include "shader/ColorTexcubeShader.h"
#include "Utils.h"


const ShaderAttribDescriptor ColorTexcubeVertex::DESCRIPTOR = DESC_NEW(ColorTexcubeVertex,
    ITEM(ColorTexcubeVertex, 0, "aPos", pos),
);

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorTexcubeShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorTexcubeShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


ColorTexcubeShader::ColorTexcubeShader()
: AbstractShader(GetShaderProg())
, _attitudeCtrl({0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}) {

}

void ColorTexcubeShader::setImage(const AbstractImage& image) {
    _renderData.setTexture("skybox", image.getTexture(ImageWrapMode::ClampToEdge));
}

void ColorTexcubeShader::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void ColorTexcubeShader::updateUniformes() {
    _renderData.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}
