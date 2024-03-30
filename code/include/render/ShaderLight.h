#ifndef _HEAD_FLAG_SHADERLIGHT_H
#define _HEAD_FLAG_SHADERLIGHT_H

#include <vector>
#include "BaseDefine.h"

struct ShaderLight {
    ShaderLight();
    void setPosition(const Position& pos);
    void setColor(const Color& color);
    const Position& getPosition() const;
    const Color& getColor() const;
    Color getAmbientColor() const;
    Color getDiffuseColor() const;
    Color getSpecularColor() const;
private:
    Position _pos;
    Color _color;
};

#endif // _HEAD_FLAG_SHADERLIGHT_H
