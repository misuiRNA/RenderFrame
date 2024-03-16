#ifndef _HEAD_FLAG_CAMERA_H
#define _HEAD_FLAG_CAMERA_H

#include <vector>

struct Camera {
    Camera();
    void enabel();
    const float* getMatrix() const;
    void setPosition(float x, float y, float z);
    void setFront(float x, float y, float z);
    void setFront(float pitch, float yaw);
    void setFov(float fov);

    const std::vector<float> getFront() const;
    const std::vector<float> getPosition() const;
    const std::vector<float> getRight() const;

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

#endif    // _HEAD_FLAG_CAMERA_H
