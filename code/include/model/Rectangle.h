#ifndef _HEAD_FLAG_MODEL_RECTANGLE_H
#define _HEAD_FLAG_MODEL_RECTANGLE_H

#include "model/AbstractModel.h"
#include "BaseDefine.h"
#include "Image.h"

struct Rectangle : AbstractModel {
    Rectangle(float width, float height);

    void setPosition(const Position& pos);
    void setFront(const Vector3D& front);
    void setSize(float width, float height);
    void setScaleRatio(float scaleRatio);
    void setImage(const Image& image);
    void setColor(const Color& color);

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    Position _pos;
    Vector3D _front;
    float _width;
    float _height;
    float _scaleRatio;
    Color _color;

    bool _imageEnable;

};

#endif
