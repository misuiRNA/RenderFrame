#ifndef _HEAD_FLAG_SHADERCAMERA_H
#define _HEAD_FLAG_SHADERCAMERA_H

#include <vector>
#include "BaseDefine.h"
#include "Attitude3DController.h"

struct ShaderCamera {
    ShaderCamera();
    const float* getMatrix() const;
    void setPosition(const Position& pos);
    void setFov(float fov);

    Attitude3DController& getMutableAttituedeCtrl();
    const Attitude3DController& getAttituedeCtrl() const;
    const Position& getPosition() const;

private:
    void updateMatrix() const;

private:
    Position _pos;
    Attitude3DController _attitudeCtrl;
    float _fov;

    mutable bool _updateMatrix;
    mutable float _matrix[16] = { 0.0f };
};

#endif // _HEAD_FLAG_SHADERCAMERA_H
