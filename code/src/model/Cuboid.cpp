#include "model/Cuboid.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cubiod::Cubiod(float size_x, float size_y, float size_z)
: AbstractModel(ShaderProgram::getCuboidShaderProg())
, _pos({0.0f, 0.0f, 0.0f})
, _size(size_x, size_y, size_z)
, _scaleRatio(1.0f)
, _rotation(0.0f)
, _imageCount(0)
, _rotationAxis({0.0f, 0.0f, 1.0f})
, _color(1.0f, 1.0f, 1.0f) {

}

void Cubiod::setPosition(const Position& pos) {
    _pos = pos;
}

void Cubiod::setSize(const Size3D& size) {
    _size = size;
}

void Cubiod::setScaleRatio(float scaleRatio) {
     _scaleRatio = scaleRatio;
}

void Cubiod::setRotation(float rotation) {
    _rotation = rotation;
}

void Cubiod::setRotationAxis(const Vector3D& axis) {
    _rotationAxis = axis;
}

void Cubiod::setColor(const Color& color) {
    _color = color;
}

// TODO: 优化, 减少重复加载
void Cubiod::addImage(const Image& image) {
    _imageCount += 1;
    if (_imageCount == 1) {
        _renderData.setTexture("texture1", image.width(), image.height(), image.data(), image.isRBGA() ? GL_RGBA : GL_RGB);
    } else if (_imageCount == 2) {
        _renderData.setTexture("texture2", image.width(), image.height(), image.data(), image.isRBGA() ? GL_RGBA : GL_RGB);
    }
}

void Cubiod::updateUniformes() {
    _prog.setUniform("imageEnable", _imageCount);
    _prog.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _prog.setUniform("material.ambient", _color.r * 0.1f, _color.g * 0.1f, _color.b * 0.1f);
    _prog.setUniform("material.diffuse", _color.r, _color.g, _color.b);
    _prog.setUniform("material.specular", 0.5f, 0.5f, 0.5f);
    _prog.setUniform("material.shininess", 32.0f);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));
    if (_rotationAxis.x != 0.0f || _rotationAxis.y != 0.0f || _rotationAxis.z != 0.0f) {
        model = glm::rotate(model, _rotation, glm::vec3(_rotationAxis.x, _rotationAxis.y, _rotationAxis.z));
    }
    model = glm::scale(model, glm::vec3(_scaleRatio * _size.x, _scaleRatio * _size.y, _scaleRatio * _size.z));

    _prog.setUniformMat4("modelMatrix", glm::value_ptr(model));
}

void Cubiod::updateRenderData() {
    _renderData.setVertices("aPos", 3, {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
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
    _renderData.setVertices("aNormal", 3, {
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,

        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,

        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,

        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,

        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f
    });
}
