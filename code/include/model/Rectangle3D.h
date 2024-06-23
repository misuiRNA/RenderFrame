#ifndef _HEAD_FLAG_MODEL_RECTANGLE_H
#define _HEAD_FLAG_MODEL_RECTANGLE_H

#include "model/AbstractDrawObject.h"
#include "BaseDefine.h"
#include "Image.h"
#include "Attitude3DController.h"

struct Rectangle3D : AbstractDrawObject {
    Rectangle3D(const Size3D& size);

    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);
    Attitude3DController& getAttituedeCtrl();

private:
    void updateRenderData() override;
    void updateUniformes() override;
    void updateModelMatrix();

private:
    Position _pos;
    Size3D _size;
    Attitude3DController _attitudeCtrl;
    Color _color;
    float _modelMatrix[16] = { 0.0f };

    bool _textureEnable;
};

#endif
