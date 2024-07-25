#ifndef _HEAD_FLAG_ATTITUDE3DCONTROLLER_H
#define _HEAD_FLAG_ATTITUDE3DCONTROLLER_H

#include "BaseDefine.h"
#include <vector>
#include <functional>


struct Attitude3DController {
    Attitude3DController(const Vector3D& up, const Vector3D& front, const Position& pos = {0.0f, 0.0f, 0.0f}, const Size3D& size = {1.0f, 1.0f, 1.0f});

    Attitude3DController& setPosition(const Position& pos);
    Attitude3DController& setFront(const Vector3D& front);
    Attitude3DController& setUp(const Vector3D& up);
    Attitude3DController& setSize(const Size3D& size);
    const Matrix4X4& getMatrix() const;

    const Position& getPosition() const;
    const Vector3D& getFront() const;
    const Vector3D& getUp() const;
    Vector3D getRight() const;
    const Size3D& getSize() const;

private:
    void attitudeChanged();

private:
    Position _pos;
    Vector3D _up;
    Vector3D _front;
    Size3D _size;

    Matrix4X4 _matrix;
};

#endif
