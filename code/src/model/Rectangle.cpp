#include "model/Rectangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Rectangle::Rectangle(float width, float height)
: _prog(ShaderProgram::getRectShaderProg())
, _renderData(_prog.buildRenderData())
, _x(0), _y(0)
, _width(width)
, _height(height)
, _scaleRatio(1.0f)
, _rotation(0.0f)
, _imageEnable(false) {
    buildModel();
}

void Rectangle::setPosition(float x, float y) {
    _x = x;
    _y = y;
}

void Rectangle::setSize(float width, float height) {
    _width = width;
    _height = height;
    buildModel();
}

void Rectangle::setScaleRatio(float scaleRatio) {
    _scaleRatio = scaleRatio;
}

void Rectangle::setRotation(float rotation) {
    _rotation = rotation;
}

void Rectangle::setImage(const std::string& filename, bool rgba) {
    Image image(filename);
    _renderData.setTexture("texture1", image.width(), image.height(), image.data(), rgba ? GL_RGBA : GL_RGB);
    _imageEnable = true;
}

void Rectangle::setColor(const Color& color) {
    _color = color;
}

void Rectangle::draw() {
    _prog.setUniform("imageEnable", _imageEnable);
    _prog.setUniform("color", _color.r, _color.g, _color.b, 1.0f);

    glm::mat4 model;
    model = glm::scale(model, glm::vec3(_scaleRatio, _scaleRatio, _scaleRatio));
    model = glm::rotate(model, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(_x, _y, 0.0f));
    _prog.setUniform("model", model.length(), glm::value_ptr(model));

    _prog.draw(_renderData);
}

void Rectangle::buildModel() {
    _renderData.setVertices("aPos", 3, {
        0,      0,       0.0f,
        _width, 0,       0.0f,
        _width, _height, 0.0f,
        0,      _height, 0.0f
    });
    _renderData.setVertices("aTexCoord", 2, {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    });
    _renderData.setIndices({
        0, 1, 2,
        0, 2, 3
    });
}
