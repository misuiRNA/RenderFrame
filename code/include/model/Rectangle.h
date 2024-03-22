#ifndef _HEAD_FLAG_MODEL_RECTANGLE_H
#define _HEAD_FLAG_MODEL_RECTANGLE_H

#include "model/AbstractModel.h"

struct Rectangle : AbstractModel {
    Rectangle(float width, float height);

    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setScaleRatio(float scaleRatio);
    void setRotation(float rotation);
    // TODO: 自动识别RGBA格式，不要手动传入
    void setImage(const std::string& filename, bool rgba = false);
    void setColor(const Color& color);

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    float _x;
    float _y;
    float _width;
    float _height;
    float _scaleRatio;
    float _rotation;
    Color _color;

    bool _imageEnable;

};

#endif
