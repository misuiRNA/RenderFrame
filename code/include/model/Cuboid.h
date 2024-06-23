#ifndef _HEAD_FLAG_MODEL_CUBOID_H
#define _HEAD_FLAG_MODEL_CUBOID_H

#include "model/AbstractDrawObject.h"
#include "BaseDefine.h"
#include "Image.h"
#include "Attitude3DController.h"

struct Cuboid : AbstractDrawObject {
    Cuboid(const Size3D& size);
    Cuboid(const Cuboid& oth);

    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setColor(const Color& color);
    void addImage(const AbstractImage& image);
    void setMaterial(const ShaderMaterial& material);
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

    int _imageCount;
};

#endif