#include "Camera.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera()
: _pos(0.0f, 0.0f, 0.0f)
, _front(0.0f, 0.0f, 1.0f)
, _fov(45.0f) {
    updateMatrix();
}

void Camera::setPosition(const Position& pos) {
    _pos = pos;
    updateMatrix();
}

void Camera::setFront(const Vector3D& front) {
    _front = front;
    updateMatrix();
}

void Camera::setFov(float fov) {
    if(fov <= 1.0f) {
        fov = 1.0f;
    } else if(fov >= 45.0f) {
        fov = 45.0f;
    }
    _fov = fov;
}

const Vector3D& Camera::getFront() const {
    return _front;
}

const Position& Camera::getPosition() const {
    return _pos;
}

const Vector3D& Camera::getUp() const {
    static Vector3D upVector { 0.0f, 0.0f,  1.0f };
    return upVector;
}

const float* Camera::getMatrix() const {
    return _matrix;
}

void Camera::updateMatrix() {
    glm::mat4 view;

    view = glm::lookAt(glm::vec3(_pos.x, _pos.y, _pos.z),
                       glm::vec3(_pos.x + _front.x, _pos.y + _front.y, _pos.z + _front.z),
                       glm::vec3(0.0f, 0.0f, 1.0f));    // Up Vector

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(_fov), 1.0f * 800 / 600, 0.1f, 100.0f);
    view = projection * view;

    memcpy(_matrix, glm::value_ptr(view), sizeof(glm::mat4));
}


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

CameraControllerFPSStyle::CameraControllerFPSStyle(Camera& camera)
: _camera(camera)
, _pitch(0.0f)
, _yaw(0.0f)
, _moveSpeed(2.5f)
, _turnSpeed(10.0f) {
    _camera.setPosition({0.0f, 0.0f, 0.0f});
    _camera.setFront({1.0f, 0.0f, 0.0f});
}

void CameraControllerFPSStyle::goForward(float moveTime) {
    glm::vec3 cameraFront = XYZ2Glmvec3(_camera.getFront());
    glm::vec3 cameraPos = XYZ2Glmvec3(_camera.getPosition());
    cameraPos += moveTime * _moveSpeed * cameraFront;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraControllerFPSStyle::goBack(float moveTime) {
    glm::vec3 cameraFront = XYZ2Glmvec3(_camera.getFront());
    glm::vec3 cameraPos = XYZ2Glmvec3(_camera.getPosition());
    cameraPos -= moveTime * _moveSpeed * cameraFront;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraControllerFPSStyle::goLeft(float moveTime) {
    glm::vec3 cameraRight = CalcRightVector(_camera.getUp(), _camera.getFront());
    glm::vec3 cameraPos = XYZ2Glmvec3(_camera.getPosition());
    cameraPos -= moveTime * _moveSpeed * cameraRight;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraControllerFPSStyle::goRight(float moveTime) {
    glm::vec3 cameraRight = CalcRightVector(_camera.getUp(), _camera.getFront());
    glm::vec3 cameraPos = XYZ2Glmvec3(_camera.getPosition());
    cameraPos += moveTime * _moveSpeed * cameraRight;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}

// TODO: 优化 相机左转右转的行为与现实世界不一致, 相机俯视时明显, 与上向量取值有关
void CameraControllerFPSStyle::turnRight(float trunTime) {
    float yaw = _yaw - trunTime * _turnSpeed;
    setAttitude(_pitch, yaw);
}
void CameraControllerFPSStyle::turnLeft(float trunTime) {
    float yaw = _yaw + trunTime * _turnSpeed;
    setAttitude(_pitch, yaw);
}
void CameraControllerFPSStyle::turnUp(float trunTime) {
    float pitch = _pitch + trunTime * _turnSpeed;
    setAttitude(pitch, _yaw);
}
void CameraControllerFPSStyle::turnDown(float trunTime) {
    float pitch = _pitch - trunTime * _turnSpeed;
    setAttitude(pitch, _yaw);
}

void CameraControllerFPSStyle::setAttitude(float pitch, float yaw) {
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
void CameraControllerFPSStyle::setPosition(float x, float y, float z) {
    _camera.setPosition({x, y, z});
}

float CameraControllerFPSStyle::normalYaw(float angel) {
    float res = angel - (int(angel / 360) * 360);
    if (res < 0.0f) {
        res += 360.0f;
    }
    return res;
}

float CameraControllerFPSStyle::normalPitch(float pitch) {
    float res = pitch;
    if (pitch > 89.0f) {
        res = 89.0f;
    } else if (pitch < -89.0f) {
        res = -89.0f;
    }
    return res;
}
