#ifndef _HEAD_FLAG_MODEL_LIGHTSOURCE_H
#define _HEAD_FLAG_MODEL_LIGHTSOURCE_H

#include "model/AbstractModel.h"

struct LightSource : AbstractModel {
    LightSource(float x, float y, float z);

    void enabel();
    void setColor(const Color& color);
    void setSize(float size_x, float size_y, float size_z);
    Position getPosition() const;
    Color getColor() const;

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    float _x;
    float _y;
    float _z;
    float _size_x;
    float _size_y;
    float _size_z;
    Color _color;
};

#endif    // _HEAD_FLAG_MODEL_LIGHTSOURCE_H