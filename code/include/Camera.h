#ifndef _HEAD_FLAG_CAMERA_H
#define _HEAD_FLAG_CAMERA_H

#include <vector>
#include "BaseDefine.h"
#include "ShaderProgram.h"

struct CameraFPS {
    CameraFPS();
    operator const ShaderCamera&() const;

    void goForward(float moveTime);
    void goBack(float moveTime);
    void goLeft(float moveTime);
    void goRight(float moveTime);

    void turnRight(float trunTime);
    void turnLeft(float trunTime);
    void turnUp(float trunTime);
    void turnDown(float trunTime);

    void setAttitude(float pitch, float yaw);
    void setPosition(float x, float y, float z);

private:
    static float normalYaw(float angel);
    static float normalPitch(float pitch);

private:
    ShaderCamera _camera;

    float _pitch;
    float _yaw;
    float _moveSpeed;
    float _turnSpeed;
};

#endif    // _HEAD_FLAG_CAMERA_H
