#include "Camera.h"
#include "ShaderProgram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static glm::vec3 XYZ2Glmvec3(const XYZ& pos) {
    glm::vec3 res = glm::vec3(pos.x, pos.y, pos.z);
    return res;
}

static glm::vec3 CalcRightVector(const XYZ& upVec, const XYZ& frontVec) {
    glm::vec3 cameraUp = XYZ2Glmvec3(upVec);
    glm::vec3 cameraFront = XYZ2Glmvec3(frontVec);
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
    return right;
}

CameraFPS::CameraFPS()
: _pitch(0.0f)
, _yaw(0.0f)
, _moveSpeed(2.5f)
, _turnSpeed(10.0f) {
    _camera.setPosition({0.0f, 0.0f, 0.0f});
    _camera.getAttituedeCtrl().setFront({1.0f, 0.0f, 0.0f});
}

CameraFPS::operator const ShaderCamera&() const
{
    return _camera;
}

void CameraFPS::goForward(float moveTime) {
    glm::vec3 cameraFront = XYZ2Glmvec3(_camera.getAttituedeCtrl().getFront());
    glm::vec3 cameraPos = XYZ2Glmvec3(_camera.getPosition());
    cameraPos += moveTime * _moveSpeed * cameraFront;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraFPS::goBack(float moveTime) {
    glm::vec3 cameraFront = XYZ2Glmvec3(_camera.getAttituedeCtrl().getFront());
    glm::vec3 cameraPos = XYZ2Glmvec3(_camera.getPosition());
    cameraPos -= moveTime * _moveSpeed * cameraFront;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraFPS::goLeft(float moveTime) {
    glm::vec3 cameraRight = CalcRightVector(_camera.getAttituedeCtrl().getUp(), _camera.getAttituedeCtrl().getFront());
    glm::vec3 cameraPos = XYZ2Glmvec3(_camera.getPosition());
    cameraPos -= moveTime * _moveSpeed * cameraRight;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraFPS::goRight(float moveTime) {
    glm::vec3 cameraRight = CalcRightVector(_camera.getAttituedeCtrl().getUp(), _camera.getAttituedeCtrl().getFront());
    glm::vec3 cameraPos = XYZ2Glmvec3(_camera.getPosition());
    cameraPos += moveTime * _moveSpeed * cameraRight;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}

// TODO: 优化 相机左转右转的行为与现实世界不一致, 相机俯视时明显, 与上向量取值有关
void CameraFPS::turnRight(float trunTime) {
    float yaw = _yaw - trunTime * _turnSpeed;
    setAttitude(_pitch, yaw);
}
void CameraFPS::turnLeft(float trunTime) {
    float yaw = _yaw + trunTime * _turnSpeed;
    setAttitude(_pitch, yaw);
}
void CameraFPS::turnUp(float trunTime) {
    float pitch = _pitch + trunTime * _turnSpeed;
    setAttitude(pitch, _yaw);
}
void CameraFPS::turnDown(float trunTime) {
    float pitch = _pitch - trunTime * _turnSpeed;
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
    _camera.getAttituedeCtrl().setFront({front.x, front.y, front.z});

}

void CameraFPS::setPosition(float x, float y, float z) {
    _camera.setPosition({x, y, z});
}

void CameraFPS::setFov(float fov) {
    _camera.setFov(fov);
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
