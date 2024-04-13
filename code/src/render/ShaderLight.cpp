#include "ShaderLight.h"


// KC: attenuationKC, KL: attenuationKL, KQ: attenuationKQ
// glsl计算光照衰减的公式 attenuation = 1.0 / (KC + KL * dist + KQ * dist * dist)
const std::map<int, Vector3D> ShaderLight::ATTENUATION_FACTOR_MAP = {
    { 7,	{1.0,	0.7,	1.8} },
    { 13,	{1.0,	0.35,	0.44} },
    { 20,	{1.0,	0.22,	0.20} },
    { 32,	{1.0,	0.14,	0.07} },
    { 50,	{1.0,	0.09,	0.032} },
    { 65,	{1.0,	0.07,	0.017} },
    { 100,	{1.0,	0.045,	0.0075} },
    { 160,	{1.0,	0.027,	0.0028} },
    { 200,	{1.0,	0.022,	0.0019} },
    { 325,	{1.0,	0.014,	0.0007} },
    { 600,	{1.0,	0.007,	0.0002} },
    { 3250,	{1.0,	0.0014,	0.000007} }
};

ShaderLight::ShaderLight()
: _spotAngle(180.0f)
, _spotOuterRatio(0.0f)
, _pos(0.0f, 0.0f, 0.0f)
, _color(1.0f, 1.0f, 1.0f)
, _spotDirection(1.0f, 0.0f, 0.0f)
,_attenuationFactor(ATTENUATION_FACTOR_MAP.at(3250)) {

}

void ShaderLight::setSpotAngle(float angle) {
    if (angle > 180.0f) {
        angle = 180.0f;
    } else if (angle < 0.0f) {
        angle = 0.0f;
    }
    _spotAngle = angle;
}

void ShaderLight::setSpotOuterRatio(float ratio) {
    if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    _spotOuterRatio = ratio;
}

void ShaderLight::setPosition(const Position& pos) {
    _pos = pos;
}

void ShaderLight::setSpotDirection(const Vector3D& dir)
{
    _spotDirection = dir;
}

void ShaderLight::setColor(const Color& color) {
    _color = color;
}

void ShaderLight::setReach(float distance) {
    int distInt = (int)distance;
    auto lower = ATTENUATION_FACTOR_MAP.lower_bound(distInt);
    auto upper = ATTENUATION_FACTOR_MAP.upper_bound(distInt);
    int diffLower = std::abs(lower->first - distInt);
    int diffUpper = std::abs(upper->first - distInt);
    int key = diffLower < diffUpper ? lower->first : upper->first;

    if (ATTENUATION_FACTOR_MAP.find(key) != ATTENUATION_FACTOR_MAP.end()) {
        _attenuationFactor = ATTENUATION_FACTOR_MAP.at(key);
    }
}

float ShaderLight::getSpotAngle() const {
    return _spotAngle;
}

float ShaderLight::getSpotOuterAngle() const {
    float res = _spotAngle * (1.0f + _spotOuterRatio);
    if (res > 180.0f) {
        res = 180.0f;
    }
    return res;
}

const Position& ShaderLight::getPosition() const {
    return _pos;
}

const Vector3D& ShaderLight::getSpotDirection() const {
    return _spotDirection;
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

const Vector3D& ShaderLight::getAttenuationFactor() const {
    return _attenuationFactor;
}



ShaderParallelLight::ShaderParallelLight()
: _dir(1.0f, 0.0f, 0.0f)
, _color(1.0f, 1.0f, 1.0f) {

}

void ShaderParallelLight::setDirection(const Vector3D& dir) {
    _dir = dir;
}

void ShaderParallelLight::setColor(const Color& color) {
    _color = color;
}

const Vector3D& ShaderParallelLight::getDirection() const {
    return _dir;
}

const Color& ShaderParallelLight::getColor() const {
    return _color;
}

Color ShaderParallelLight::getAmbientColor() const {
    return _color * 0.5f;
}

Color ShaderParallelLight::getDiffuseColor() const {
    return _color * 0.8f;
}

Color ShaderParallelLight::getSpecularColor() const {
    return _color * 1.0f;
}
