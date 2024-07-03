#include "model/Rectangle3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
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
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",      0, Rectangle3DVertex, pos),
        DESC("aTexCoord", 1, Rectangle3DVertex, texcoord),
    };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, descriptor);
    return prog;
}


Rectangle3D::Rectangle3D(const Size3D& size)
: AbstractDrawObject(GetShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _size(size)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f})
, _textureEnable(false) {
    updateModelMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this](){ updateModelMatrix(); });
}

void Rectangle3D::setPosition(const Position& pos) {
    _pos = pos;
    updateModelMatrix();
}

void Rectangle3D::setSize(const Size3D& size) {
    _size = size;
    updateModelMatrix();
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
    _renderData.setUniformMat4("modelMatrix", _modelMatrix);
}

Attitude3DController& Rectangle3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void Rectangle3D::updateRenderData() {
    std::vector<Rectangle3DVertex> vertices = {
        {{-0.5f, -0.5f, 0.0f},  {0.0f, 0.0f}},
        {{0.5f,  -0.5f, 0.0f},  {1.0f, 0.0f}},
        {{0.5f,  0.5f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f, 0.5f,  0.0f},  {0.0f, 1.0f}},
    };
    _renderData.setVertices(vertices);

    _renderData.setIndices({
        0, 1, 2,
        0, 2, 3
    });
}

void Rectangle3D::updateModelMatrix() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));

    glm::mat4 attitudeMatrix = _attitudeCtrl.getAttitudeMatrix();
    model = model * attitudeMatrix;

    model = glm::scale(model, glm::vec3(_size.x,  _size.y, 1.0f));

    memcpy(_modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
}
