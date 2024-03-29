#ifndef _HEAD_FLAG_MODEL_LIGHTSOURCE_H
#define _HEAD_FLAG_MODEL_LIGHTSOURCE_H

#include "model/AbstractModel.h"
#include "BaseDefine.h"

struct LightSource : AbstractModel {
    LightSource(float x, float y, float z);

    void setColor(const Color& color);
    void setSize(float size_x, float size_y, float size_z);
    void setPosition(const Position& pos);
    const Position& getPosition() const;
    Color getColor() const;

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    Position _pos;
    float _size_x;
    float _size_y;
    float _size_z;
    Color _color;
};

#endif    // _HEAD_FLAG_MODEL_LIGHTSOURCE_H