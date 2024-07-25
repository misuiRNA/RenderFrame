#include "Camera.h"
#include "ShaderProgram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


CameraFPS::CameraFPS()
: _pitch(0.0f)
, _yaw(0.0f) {
    _camera.setPosition({0.0f, 0.0f, 0.0f});
    _camera.setFront({1.0f, 0.0f, 0.0f});
}

CameraFPS::operator const ShaderCamera&() const
{
    return _camera;
}

void CameraFPS::move(const Vector3D& vec) {
    _camera.move(vec);
}

void CameraFPS::goForward(float stepLen) {
    move(stepLen * _camera.getAttituedeCtrl().getFront());
}

void CameraFPS::goBack(float stepLen) {
    move(- stepLen * _camera.getAttituedeCtrl().getFront());
}

void CameraFPS::goLeft(float stepLen) {
    move(- stepLen * _camera.getAttituedeCtrl().getRight());
}

void CameraFPS::goRight(float stepLen) {
    move(stepLen * _camera.getAttituedeCtrl().getRight());
}

// TODO: 优化 相机左转右转的行为与现实世界不一致, 相机俯视时明显, 与上向量取值有关
void CameraFPS::turnRight(float stepAngle) {
    float yaw = _yaw - stepAngle;
    setAttitude(_pitch, yaw);
}

void CameraFPS::turnLeft(float stepAngle) {
    float yaw = _yaw + stepAngle;
    setAttitude(_pitch, yaw);
}

void CameraFPS::turnUp(float stepAngle) {
    float pitch = _pitch + stepAngle;
    setAttitude(pitch, _yaw);
}

void CameraFPS::turnDown(float stepAngle) {
    float pitch = _pitch - stepAngle;
    setAttitude(pitch, _yaw);
}

void CameraFPS::setAttitude(float pitch, float yaw) {
    yaw = normalYaw(yaw);
    pitch = normalPitch(pitch);

    _pitch = pitch;
    _yaw = yaw;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front.z = sin(glm::radians(pitch));
    front = glm::normalize(front);
    _camera.setFront({front.x, front.y, front.z});

}

void CameraFPS::setPosition(const Position& pos) {
    _camera.setPosition(pos);
}

void CameraFPS::setFov(float fov) {
    _camera.setFov(fov);
}

Position CameraFPS::getPosition() const {
    return _camera.getPosition();
}

float CameraFPS::normalYaw(float angel) {
    float res = angel - (int(angel / 360) * 360);
    if (res < 0.0f) {
        res += 360.0f;
    }
    return res;
}

float CameraFPS::normalPitch(float pitch) {
    float res = pitch;
    if (pitch > 89.0f) {
        res = 89.0f;
    } else if (pitch < -89.0f) {
        res = -89.0f;
    }
    return res;
}
