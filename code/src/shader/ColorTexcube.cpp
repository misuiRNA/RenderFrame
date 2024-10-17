#include "shader/ColorTexcube.h"
#include "Utils.h"


const ShaderAttribDescriptor ColorTexcubeVertex::DESCRIPTOR = DESC_NEW(ColorTexcubeVertex,
    ITEM(ColorTexcubeVertex, 0, "aPos", pos),
);

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorTexcubeShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorTexcubeShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


ColorTexcube::ColorTexcube()
: AbstractShader(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}) {

}

void ColorTexcube::setImage(const AbstractImage& image) {
    _renderData.setTexture("skybox", image.getTexture(ImageWrapMode::ClampToEdge));
}

void ColorTexcube::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void ColorTexcube::updateUniformes() {
    _renderData.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}
