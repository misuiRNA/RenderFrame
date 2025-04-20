#ifndef _HEAD_FLAG_CAMERA_H
#define _HEAD_FLAG_CAMERA_H

#include <vector>
#include "base/BaseDefine.h"
#include "ShaderProgram.h"

struct CameraFPS {
    CameraFPS();
    operator const ShaderCamera&() const;

    void move(const Vector3D& vec);
    void goForward(float stepLen);
    void goBack(float stepLen);
    void goLeft(float stepLen);
    void goRight(float stepLen);

    void turnRight(float stepAngle);
    void turnLeft(float stepAngle);
    void turnUp(float stepAngle);
    void turnDown(float stepAngle);

    void setAttitude(float pitch, float yaw);
    void setPosition(const Position& pos);
    void setFov(float fov);

    Position getPosition() const;
    Vector3D getDirection() const;

private:

private:
    static float normalYaw(float angel);
    static float normalPitch(float pitch);

private:
    ShaderCamera _camera;

    float _pitch;
    float _yaw;
};

#endif    // _HEAD_FLAG_CAMERA_H
