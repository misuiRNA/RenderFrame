#include "shader/ColorTex3DShader.h"
#include "ShaderProgram.h"
#include "base/Utils.h"
#include "assets/UtilsFileSys.h"


const ShaderAttribDescriptor ColorTex3DShader::DESCRIPTOR = DESC_NEW(ColorTex3DVertex,
    ITEM(ColorTex3DVertex, 0, "aPos",      pos),
    ITEM(ColorTex3DVertex, 1, "aTexCoord", texcoord),
    ITEM(ColorTex3DVertex, 2, "aColor", color),
);

// TODO: 优化, 1.shader字符串编译时确定，不读取文件；2.返回的路径位置应为可执行文件位置，而不是执行命令的位置 考虑使用 std::filesystem
static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorTex3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorTex3DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


ColorTex3DShader::ColorTex3DShader(const Size3D& size)
: AbstractShader(GetShaderProg())
, _trans({0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, size)
, _textureEnable(false) {

}

void ColorTex3DShader::setPosition(const Position& pos) {
    _trans.setPosition(pos);
}

void ColorTex3DShader::setSize(const Size3D& size) {
    _trans.setSize(size);
}

void ColorTex3DShader::setImage(const AbstractImage& image) {
    setTexture("texture1", image.getTexture(ImageWrapMode::ClampToEdge));
    _textureEnable = true;
}

void ColorTex3DShader::setColor(const Color& color) {
    _color = color;
}

void ColorTex3DShader::updateUniformes() {
    setUniform("imageEnable", _textureEnable);
    setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    setUniform("modelMatrix", _trans.getMatrix());
}

Transform& ColorTex3DShader::getTransform() {
    return _trans;
}
