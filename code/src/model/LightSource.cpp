#include "model/LightSource.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"

LightSource::LightSource(bool isParallel)
: AbstractModel(ShaderProgram::GetLightSourceShaderProg())
, _shaderLight(isParallel)
, _size(1.0f, 1.0f, 1.0f) {

}

LightSource::operator const ShaderLight&() const {
    return _shaderLight;
}

void LightSource::setColor(const Color& color) {
    _shaderLight.setColor(color);
}

void LightSource::setSize(const Size3D& size) {
    _size = size;
}

void LightSource::setSpotFacor(float angle, float expandSmoothRatio) {
    _shaderLight.setSpotAngle(angle);
    _shaderLight.setSpotOuterRatio(expandSmoothRatio);
}

void LightSource::setPosition(const Position& pos) {
    _shaderLight.setPosition(pos);
}

void LightSource::setDirection(const Vector3D& dir) {
    _shaderLight.setSpotDirection(dir);
}

void LightSource::setReach(float distance) {
    _shaderLight.setReach(distance);
}

const Position& LightSource::getPosition() const {
    return _shaderLight.getPosition();
}

Color LightSource::getColor() const {
    return _shaderLight.getColor();
}

bool LightSource::isParallel() const {
    return _shaderLight.isParallel();
}

void LightSource::updateUniformes() {
    const Color& orgColor = _shaderLight.getColor();
    _renderData.setUniform("color", orgColor.r, orgColor.g, orgColor.b, 1.0f);

    glm::mat4 model;
    const Position& pos = _shaderLight.getPosition();
    model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
    model = glm::scale(model, glm::vec3(_size.x, _size.y, _size.z));
    _renderData.setUniformMat4("modelMatrix", glm::value_ptr(model));
}

void LightSource::updateRenderData() {
    _renderData.setVertices<Vector3D>("aPos", {
        {-0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},

        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},

        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},

        {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},

        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, -0.5f},

        {-0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, 0.5f},
    });
}
