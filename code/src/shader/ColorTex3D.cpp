#include "shader/ColorTex3D.h"
#include "ShaderProgram.h"
#include "Utils.h"


// TODO: 优化, 1.shader字符串编译时确定，不读取文件；2.返回的路径位置应为可执行文件位置，而不是执行命令的位置 考虑使用 std::filesystem
static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorTex3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorTex3DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


ColorTex3D::ColorTex3D(const Size3D& size)
: AbstractShader(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, size)
, _textureEnable(false) {

}

void ColorTex3D::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void ColorTex3D::setSize(const Size3D& size) {
    _attitudeCtrl.setSize(size);
}

void ColorTex3D::setImage(const AbstractImage& image) {
    _renderData.setTexture("texture1", image.getTexture(ImageWrapMode::ClampToEdge));
    _textureEnable = true;
}

void ColorTex3D::setColor(const Color& color) {
    _color = color;
}

void ColorTex3D::setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",      0, Vertex, pos),
        DESC("aTexCoord", 1, Vertex, texcoord),
    };

    _renderData.setVertices(vertices, descriptor);

    if (!indices.empty()) {
        _renderData.setIndices(indices);
    }
}

void ColorTex3D::updateUniformes() {
    _renderData.setUniform("imageEnable", _textureEnable);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _renderData.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}

Attitude3DController& ColorTex3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}
