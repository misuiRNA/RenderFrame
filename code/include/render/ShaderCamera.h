#ifndef _HEAD_FLAG_SHADERCAMERA_H
#define _HEAD_FLAG_SHADERCAMERA_H

#include <vector>
#include "BaseDefine.h"

struct ShaderCamera {
    ShaderCamera();
    const float* getMatrix() const;
    void setPosition(const Position& pos);
    void setFront(const Vector3D& front);
    void setFov(float fov);

    const Position& getPosition() const;
    const Vector3D& getFront() const;
    const Vector3D& getUp() const;

private:
    void updateMatrix();

private:
    Position _pos;
    Vector3D _front;
    float _fov;
    float _matrix[16] = { 0.0f };
};

#endif // _HEAD_FLAG_SHADERCAMERA_H
