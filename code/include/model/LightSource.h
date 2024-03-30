#ifndef _HEAD_FLAG_MODEL_LIGHTSOURCE_H
#define _HEAD_FLAG_MODEL_LIGHTSOURCE_H

#include "model/AbstractModel.h"
#include "BaseDefine.h"

struct LightSource : AbstractModel {
    LightSource(float x, float y, float z);
    operator const RenderLight&() const;

    void setColor(const Color& color);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    const Position& getPosition() const;
    Color getColor() const;

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    RenderLight _renderLight;
    Size3D _size;
};

#endif    // _HEAD_FLAG_MODEL_LIGHTSOURCE_H