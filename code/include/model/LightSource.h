#ifndef _HEAD_FLAG_MODEL_LIGHTSOURCE_H
#define _HEAD_FLAG_MODEL_LIGHTSOURCE_H

#include "model/AbstractModel.h"
#include "BaseDefine.h"
#include "ShaderLight.h"

struct LightSource : AbstractModel {
    LightSource(bool isParallel);
    operator const ShaderLight&() const;

    void setColor(const Color& color);
    void setPosition(const Position& pos);
    void setDirection(const Vector3D& dir);
    void setSize(const Size3D& size);
    void setSpotFacor(float angle, float expandSmoothRatio = 0.1f);
    void setReach(float distance);
    const Position& getPosition() const;
    Color getColor() const;
    bool isParallel() const;

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    ShaderLight _shaderLight;
    Size3D _size;
};

#endif    // _HEAD_FLAG_MODEL_LIGHTSOURCE_H