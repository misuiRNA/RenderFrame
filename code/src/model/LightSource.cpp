#include "model/LightSource.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

LightSource::LightSource(float x, float y, float z)
: AbstractModel(ShaderProgram::getLightSourceShaderProg())
, _x(x)
, _y(y)
, _z(z)
, _size_x(1.0f)
, _size_y(1.0f)
, _size_z(1.0f)
, _color(1.0f, 1.0f, 1.0f) {

}

void LightSource::enabel() {
    for (auto itr : ShaderProgram::getAllShaderProg())
    {
        if (itr.first) {
            ShaderProgram& prog = *itr.first;

            prog.setUniform("light.pos", _x, _y, _z);
            prog.setUniform("light.ambient",  _color.r * 0.2f, _color.g * 0.2f, _color.b * 0.2f);
            prog.setUniform("light.diffuse",  _color.r * 0.8f, _color.g * 0.8f, _color.b * 0.8f);
            prog.setUniform("light.specular", _color.r * 1.0f, _color.g * 1.0f, _color.b * 1.0f);
        }
    }
}

void LightSource::setColor(const Color& color) {
    _color = color;
}

void LightSource::setSize(float size_x, float size_y, float size_z) {
    _size_x = size_x;
    _size_y = size_y;
    _size_z = size_z;
}

void LightSource::setPosition(const Position& pos) {
    _x = pos.x;
    _y = pos.y;
    _z = pos.z;
}

Position LightSource::getPosition() const {
    return Position(_x, _y, _z);
}

Color LightSource::getColor() const {
    return _color;
}

void LightSource::updateUniformes() {
    _prog.setUniform("color", _color.r, _color.g, _color.b, 1.0f);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_x, _y, _z));
    model = glm::scale(model, glm::vec3(_size_x, _size_y, _size_z));
    _prog.setUniformMat4("modelMatrix", glm::value_ptr(model));
}

void LightSource::updateRenderData() {
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
}
