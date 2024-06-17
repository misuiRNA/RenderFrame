#include "model/Rectangle3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"

Rectangle3D::Rectangle3D(float width, float height)
: AbstractModel(ShaderProgram::GetRectShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f})
, _width(width)
, _height(height)
, _scaleRatio(1.0f)
, _textureEnable(false) {
    updateModelMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this](){ updateModelMatrix(); });
}

void Rectangle3D::setPosition(const Position& pos) {
    _pos = pos;
    updateModelMatrix();
}

void Rectangle3D::setSize(float width, float height) {
    _width = width;
    _height = height;
    updateModelMatrix();
}

void Rectangle3D::setScaleRatio(float scaleRatio) {
    _scaleRatio = scaleRatio;
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

    model = glm::scale(model, glm::vec3(_scaleRatio * _width, _scaleRatio * _height, 1.0f));

    memcpy(_modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
}
