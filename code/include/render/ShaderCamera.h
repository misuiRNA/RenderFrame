#ifndef _HEAD_FLAG_SHADERCAMERA_H
#define _HEAD_FLAG_SHADERCAMERA_H

#include <vector>
#include "base/BaseDefine.h"
#include "base/Transform.h"

struct ShaderCamera {
    ShaderCamera();
    ShaderCamera(const ShaderCamera& oth);

    const Matrix4X4& getMatrix() const;
    void setPosition(const Position& pos);
    void move(const Vector3D& vec);
    void setFront(const Vector3D& front);
    void setFov(float fov);

    const Transform& getTransform() const;
    const Position& getPosition() const;

private:
    void updateMatrix();

private:
    Transform _trans;
    float _fov;

    Matrix4X4 _matrix;
};

#endif // _HEAD_FLAG_SHADERCAMERA_H
