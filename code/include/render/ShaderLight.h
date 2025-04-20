#ifndef _HEAD_FLAG_SHADERLIGHT_H
#define _HEAD_FLAG_SHADERLIGHT_H

#include <vector>
#include <map>
#include "base/BaseDefine.h"

struct ShaderLight {
    ShaderLight(bool isParallel);
    void setSpotAngle(float angle);
    void setSpotOuterRatio(float ratio);
    void setPosition(const Position& pos);
    void setSpotDirection(const Vector3D& dir);
    void setColor(const Color& color);
    void setReach(float distance);
    float getSpotAngle() const;
    float getSpotOuterAngle() const;
    const Position& getPosition() const;
    const Vector3D& getSpotDirection() const;
    const Color& getColor() const;
    Color getAmbientColor() const;
    Color getDiffuseColor() const;
    Color getSpecularColor() const;
    const Vector3D& getAttenuationFactor() const;
    bool isParallel() const;
private:
    float _spotAngle;
    float _spotOuterRatio;
    Position _pos;
    Color _color;
    Vector3D _spotDirection;
    Vector3D _attenuationFactor;
    bool _isParallel;

    static const std::map<int, Vector3D> ATTENUATION_FACTOR_MAP;
};

#endif // _HEAD_FLAG_SHADERLIGHT_H
