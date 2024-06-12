#ifndef _HEAD_FLAG_MODEL_RECTANGLE_H
#define _HEAD_FLAG_MODEL_RECTANGLE_H

#include "model/AbstractModel.h"
#include "BaseDefine.h"
#include "Image.h"
#include "Attitude3DController.h"

struct Rectangle3D : AbstractModel {
    Rectangle3D(float width, float height);

    void setPosition(const Position& pos);
    void setSize(float width, float height);
    void setScaleRatio(float scaleRatio);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);
    Attitude3DController& getAttituedeCtrl();

private:
    void updateRenderData() override;
    void updateUniformes() override;
    void updateModelMatrix();

private:
    Position _pos;
    Attitude3DController _attitudeCtrl;
    float _width;
    float _height;
    float _scaleRatio;
    Color _color;
    float _modelMatrix[16] = { 0.0f };

    bool _textureEnable;
};

#endif
