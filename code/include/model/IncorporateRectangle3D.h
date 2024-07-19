#ifndef _HEAD_FLAG_MODEL_INCORPORATERECTANGLE3D_H
#define _HEAD_FLAG_MODEL_INCORPORATERECTANGLE3D_H

#include "model/AbstractDrawObject.h"
#include "BaseDefine.h"
#include "Image.h"
#include "Attitude3DController.h"

struct IncorporateRectangle3D : AbstractDrawObject {
    IncorporateRectangle3D(const Size3D& size);
    IncorporateRectangle3D(const IncorporateRectangle3D& oth);

    // TODO: 优化, 考虑优化设计提升opengl实例化属性的可复用性
    void mergeCopies(std::vector<IncorporateRectangle3D>& rectangles);
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
    Matrix4X4 _modelMatrix;

    bool _textureEnable;
};

#endif
