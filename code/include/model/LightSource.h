#ifndef _HEAD_FLAG_MODEL_LIGHTSOURCE_H
#define _HEAD_FLAG_MODEL_LIGHTSOURCE_H

#include "model/AbstractModel.h"
#include "BaseDefine.h"

struct LightSource : AbstractModel {
    LightSource(float x, float y, float z);

    void setColor(const Color& color);
    void setSize(const Size3D& size);
    void setPosition(const Position& pos);
    const Position& getPosition() const;
    Color getColor() const;

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    Position _pos;
    Size3D _size;
    Color _color;
};

#endif    // _HEAD_FLAG_MODEL_LIGHTSOURCE_H