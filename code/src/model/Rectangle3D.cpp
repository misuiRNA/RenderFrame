#include "model/Rectangle3D.h"
#include "ShaderProgram.h"
#include "Utils.h"

struct Rectangle3DVertex {
    Position pos;
    Vector2D texcoord;
};

// TODO: 优化, 1.shader字符串编译时确定，不读取文件；2.返回的路径位置应为可执行文件位置，而不是执行命令的位置 考虑使用 std::filesystem
static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle3DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


Rectangle3D::Rectangle3D(const Size3D& size)
: AbstractDrawObject(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, size)
, _textureEnable(false) {

}

void Rectangle3D::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void Rectangle3D::setSize(const Size3D& size) {
    _attitudeCtrl.setSize(size);
}

void Rectangle3D::setImage(const AbstractImage& image) {
    _renderData.setTexture("texture1", image.getTexture(ImageWrapMode::ClampToEdge));
    _textureEnable = true;
}

void Rectangle3D::setColor(const Color& color) {
    _color = color;
}

void Rectangle3D::updateUniformes() {
    _renderData.setUniform("imageEnable", _textureEnable);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _renderData.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}

Attitude3DController& Rectangle3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void Rectangle3D::updateRenderData() {
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",      0, Rectangle3DVertex, pos),
        DESC("aTexCoord", 1, Rectangle3DVertex, texcoord),
    };

    std::vector<Rectangle3DVertex> vertices = {
        {{-0.5f, -0.5f, 0.0f},  {0.0f, 0.0f}},
        {{0.5f,  -0.5f, 0.0f},  {1.0f, 0.0f}},
        {{0.5f,  0.5f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f, 0.5f,  0.0f},  {0.0f, 1.0f}},
    };
    _renderData.setVertices(vertices, descriptor);

    _renderData.setIndices({
        0, 1, 2,
        0, 2, 3
    });
}
