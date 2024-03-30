#include "ShaderLight.h"

ShaderLight::ShaderLight() : _pos(0.0f, 0.0f, 0.0f), _color(1.0f, 1.0f, 1.0f) {

}

void ShaderLight::setPosition(const Position& pos) {
    _pos = pos;
}

void ShaderLight::setColor(const Color& color) {
    _color = color;
}

const Position& ShaderLight::getPosition() const {
    return _pos;
}

const Color& ShaderLight::getColor() const {
    return _color;
}

Color ShaderLight::getAmbientColor() const {
    return _color * 0.2f;
}

Color ShaderLight::getDiffuseColor() const {
    return _color * 0.8f;
}

Color ShaderLight::getSpecularColor() const {
    return _color * 1.0f;
}
