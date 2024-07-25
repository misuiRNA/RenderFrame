#ifndef _HEAD_FLAG_SHADERCAMERA_H
#define _HEAD_FLAG_SHADERCAMERA_H

#include <vector>
#include "BaseDefine.h"
#include "Attitude3DController.h"

struct ShaderCamera {
    ShaderCamera();
    ShaderCamera(const ShaderCamera& oth);

    const Matrix4X4& getMatrix() const;
    void setPosition(const Position& pos);
    void move(const Vector3D& vec);
    void setFront(const Vector3D& front);
    void setFov(float fov);

    const Attitude3DController& getAttituedeCtrl() const;
    const Position& getPosition() const;

private:
    void updateMatrix();

private:
    Attitude3DController _attitudeCtrl;
    float _fov;

    Matrix4X4 _matrix;
};

#endif // _HEAD_FLAG_SHADERCAMERA_H
