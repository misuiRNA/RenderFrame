#include "model/Rectangle3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Utils.h"

// TODO: 优化, 1.shader字符串编译时确定，不读取文件；2.返回的路径位置应为可执行文件位置，而不是执行命令的位置 考虑使用 std::filesystem
static ShaderProgram& GetShaderProg() {
    static const std::string MODEL_NAME = "Rectangle3D";
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle3DShader.fs");
    static const std::map<std::string, int> ATTRIBUTE_NAME_MAP = {
        {"aPos"     , 0},
        {"aTexCoord", 1},
    };

    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, ATTRIBUTE_NAME_MAP);
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
    _renderData.setVertices<Vector3D>("aPos", {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
    });
    _renderData.setVertices<Vector2D>("aTexCoord", {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
    });
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
