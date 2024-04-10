#ifndef _HEAD_FLAG_MODEL_CUBOID_H
#define _HEAD_FLAG_MODEL_CUBOID_H

#include "model/AbstractModel.h"
#include "BaseDefine.h"
#include "Image.h"

struct Cubiod : AbstractModel {
    Cubiod(float size_x, float size_y, float size_z);

    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setScaleRatio(float scaleRatio);
    void setRotation(float rotation);
    void setRotationAxis(const Vector3D& axis);
    void setColor(const Color& color);
    void addImage(const Image& image);
    void setMaterialImage(const Image& image);
    void setMaterialSpecularImage(const Image& image);

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    Position _pos;
    Size3D _size;
    float _scaleRatio;
    float _rotation;
    Vector3D _rotationAxis;
    Color _color;

    int _imageCount;
};

#endif