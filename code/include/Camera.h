#ifndef _HEAD_FLAG_CAMERA_H
#define _HEAD_FLAG_CAMERA_H

#include <vector>

struct Camera {
    Camera();
    void enabel();
    const float* getMatrix() const;
    void setPosition(float x, float y, float z);
    void setFront(float x, float y, float z);
    void setFov(float fov);

    const std::vector<float> getFront() const;
    const std::vector<float> getPosition() const;
    const std::vector<float> getUp() const;

private:
    void updateMatrix();

private:
    float _x;
    float _y;
    float _z;
    float _frontX;
    float _frontY;
    float _frontZ;
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
