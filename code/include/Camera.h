#ifndef _HEAD_FLAG_CAMERA_H
#define _HEAD_FLAG_CAMERA_H

#include <vector>
#include "BaseDefine.h"

struct Camera {
    Camera();
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


struct CameraControllerFPSStyle {
    CameraControllerFPSStyle(Camera& camera);

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
    Camera& _camera;

    float _pitch;
    float _yaw;
    float _moveSpeed;
    float _turnSpeed;
};

#endif    // _HEAD_FLAG_CAMERA_H
