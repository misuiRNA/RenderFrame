#ifndef _HEAD_FLAG_MODEL_RECTANGLE_H
#define _HEAD_FLAG_MODEL_RECTANGLE_H

#include "model/AbstractModel.h"
#include "BaseDefine.h"
#include "Image.h"

struct Rectangle : AbstractModel {
    Rectangle(float width, float height);

    void setPosition(const Position& pos);
    void setSize(float width, float height);
    void setScaleRatio(float scaleRatio);
    void setRotation(float rotation);
    void setImage(const Image& image);
    void setColor(const Color& color);

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    Position _pos;
    float _width;
    float _height;
    float _scaleRatio;
    float _rotation;
    Color _color;

    bool _imageEnable;

};

#endif
