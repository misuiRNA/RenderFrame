#include "model/LightSource.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

LightSource::LightSource(float x, float y, float z)
: AbstractModel(ShaderProgram::getLightSourceShaderProg())
, _size(1.0f, 1.0f, 1.0f) {
    _renderLight.setPosition({x, y, z});
}

LightSource::operator const RenderLight&() const {
    return _renderLight;
}

void LightSource::setColor(const Color& color) {
    _renderLight.setColor(color);
}

void LightSource::setSize(const Size3D& size) {
    _size = size;
}

void LightSource::setPosition(const Position& pos) {
    _renderLight.setPosition(pos);
}

const Position& LightSource::getPosition() const {
    return _renderLight.getPosition();
}

Color LightSource::getColor() const {
    return _renderLight.getColor();
}

void LightSource::updateUniformes() {
    const Color& orgColor = _renderLight.getColor();
    _prog.setUniform("color", orgColor.r, orgColor.g, orgColor.b, 1.0f);

    glm::mat4 model;
    const Position& pos = _renderLight.getPosition();
    model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
    model = glm::scale(model, glm::vec3(_size.x, _size.y, _size.z));
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
