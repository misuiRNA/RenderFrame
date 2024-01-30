#include "model/Cuboid.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Image.h"

Cubiod::Cubiod(float size_x, float size_y, float size_z)
: AbstractModel(ShaderProgram::getCuboidShaderProg())
, _size_x(size_x)
, _size_y(size_y)
, _size_z(size_z)
, _scaleRatio(1.0f)
, _rotation(0.0f)
, _imageCount(0) {
    setRotationAxis({0.0f, 0.0f, 0.0f});
    buildModel();
}

void Cubiod::setPosition(float x, float y, float z) {
    _x = x;
    _y = y;
    _z = z;
}

void Cubiod::setSize(float size_x, float size_y, float size_z) {
    _size_x = size_x;
    _size_y = size_y;
    _size_z = size_z;
}

void Cubiod::setScaleRatio(float scaleRatio) {
     _scaleRatio = scaleRatio;
}

void Cubiod::setRotation(float rotation) {
    _rotation = rotation;
}

void Cubiod::setRotationAxis(const float (&axis)[3]) {
    _rotationAxis[0] = axis[0];
    _rotationAxis[1] = axis[1];
    _rotationAxis[2] = axis[2];
}

void Cubiod::addImage(const std::string& filename, bool rgba) {
    Image image(filename);
    _imageCount += 1;
    if (_imageCount == 1) {
        _renderData.setTexture("texture1", image.width(), image.height(), image.data(), rgba ? GL_RGBA : GL_RGB);
    } else if (_imageCount == 2) {
        _renderData.setTexture("texture2", image.width(), image.height(), image.data(), rgba ? GL_RGBA : GL_RGB);
    }
}

void Cubiod::draw() {
    // TODO: 优化，view和projection不属于物体本身的属性，需要解耦
    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1.0f * 800 / 600, 0.1f, 100.0f);

    glm::mat4 model;
    model = glm::scale(model, glm::vec3(_scaleRatio * _size_x, _scaleRatio * _size_y, _scaleRatio * _size_z));
    if (_rotationAxis[0] != 0.0f || _rotationAxis[1] != 0.0f || _rotationAxis[2] != 0.0f) {
        model = glm::rotate(model, _rotation, glm::vec3(_rotationAxis[0], _rotationAxis[1], _rotationAxis[2]));
    }
    model = glm::translate(model, glm::vec3(_x, _y, _z));

    _prog.setUniform("model", model.length(), glm::value_ptr(model));
    _prog.setUniform("view", view.length(), glm::value_ptr(view));
    _prog.setUniform("projection", projection.length(), glm::value_ptr(projection));

    AbstractModel::draw();
}

void Cubiod::buildModel() {
    _renderData.setVertices("aPos", 3, {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
    });
    _renderData.setVertices("aTexCoord", 2, {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    });
}
