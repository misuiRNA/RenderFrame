#include "model/Rectangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"

Rectangle::Rectangle(float width, float height)
: AbstractModel(ShaderProgram::getRectShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _width(width)
, _height(height)
, _scaleRatio(1.0f)
, _rotation(0.0f)
, _imageEnable(false) {

}

void Rectangle::setPosition(const Position& pos) {
    _pos = pos;
}

void Rectangle::setSize(float width, float height) {
    _width = width;
    _height = height;
}

void Rectangle::setScaleRatio(float scaleRatio) {
    _scaleRatio = scaleRatio;
}

void Rectangle::setRotation(float rotation) {
    _rotation = rotation;
}

void Rectangle::setImage(const Image& image) {
    _renderData.setTexture("texture1", image.getTexture());
    _imageEnable = true;
}

void Rectangle::setColor(const Color& color) {
    _color = color;
}

void Rectangle::updateUniformes() {
    _renderData.setUniform("imageEnable", _imageEnable);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));
    model = glm::rotate(model, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(_scaleRatio * _width, _scaleRatio * _height, 1.0f));
    _renderData.setUniformMat4("modelMatrix", glm::value_ptr(model));
}

void Rectangle::updateRenderData() {
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
