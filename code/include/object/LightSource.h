#ifndef _HEAD_FLAG_MODEL_LIGHTSOURCE_H
#define _HEAD_FLAG_MODEL_LIGHTSOURCE_H

#include "shader/Color3DShader.h"


struct LightSource {
    LightSource(bool isParallel);
    operator const ShaderLight&() const;
    void setColor(const Color& color);
    void setPosition(const Position& pos);
    void setDirection(const Vector3D& dir);
    void setSize(const Size3D& size);
    void setSpotFacor(float angle, float expandSmoothRatio = 0.1f);
    void setReach(float distance);
    void setVertexData(const RenderShape& shape);
    const Position& getPosition() const;
    Color getColor() const;
    bool isParallel() const;
    void show();

private:
    Color3DShader _shader;
    ShaderLight _shaderLight;
};

#endif