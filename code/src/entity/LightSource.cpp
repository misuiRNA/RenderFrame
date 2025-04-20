#include "entity/LightSource.h"

LightSource::LightSource(bool isParallel)
: _shaderLight(isParallel)
, _shader(isParallel) {

}

LightSource::operator const ShaderLight&() const {
    return _shaderLight;
}

void LightSource::setColor(const Color& color) {
    _shaderLight.setColor(color);
    _shader.setColor(color);
}

void LightSource::setPosition(const Position& pos) {
    _shaderLight.setPosition(pos);
    _shader.setPosition(pos);
}

void LightSource::setDirection(const Vector3D& dir) {
    _shaderLight.setSpotDirection(dir);
}
void LightSource::setSize(const Size3D& size) {
    _shader.setSize(size);
}

void LightSource::setSpotFacor(float angle, float expandSmoothRatio) {
    _shaderLight.setSpotAngle(angle);
    _shaderLight.setSpotOuterRatio(expandSmoothRatio);
}

void LightSource::setReach(float distance) {
    _shaderLight.setReach(distance);
}

void LightSource::setVertexData(const RenderShape& shape) {
    _shader.setVertexData(shape);
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

void LightSource::show() {
    _shader.show();
}
