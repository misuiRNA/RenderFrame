#include "Camera.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera()
: _x(0.0f)
, _y(0.0f)
, _z(0.0f)
, _frontX(0.0f)
, _frontY(0.0f)
, _frontZ(0.0f)
, _fov(45.0f) {
    updateMatrix();
}

void Camera::setPosition(float x, float y, float z) {
    _x = x;
    _y = y;
    _z = z;
    updateMatrix();
}

void Camera::setFront(float x, float y, float z) {
    _frontX = x;
    _frontY = y;
    _frontZ = z;
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

const std::vector<float> Camera::getFront() const {
    return std::vector<float>{_frontX, _frontY, _frontZ};
}

const std::vector<float> Camera::getPosition() const {
    return std::vector<float>{_x, _y, _z};
}

const std::vector<float> Camera::getUp() const {
    return std::vector<float> {0.0f, 0.0f,  1.0f};
}

const float* Camera::getMatrix() const {
    return _matrix;
}

void Camera::updateMatrix() {
    glm::mat4 view;

    view = glm::lookAt(glm::vec3(_x, _y, _z),
                       glm::vec3(_x + _frontX, _y + _frontY, _z + _frontZ),
                       glm::vec3(0.0f, 0.0f, 1.0f));    // Up Vector

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(_fov), 1.0f * 800 / 600, 0.1f, 100.0f);
    view = projection * view;

    memcpy(_matrix, glm::value_ptr(view), sizeof(glm::mat4));
}

void Camera::enabel() {
    for (auto itr : ShaderProgram::getAllShaderProg())
    {
        if (itr.first) {
            ShaderProgram& prog = *itr.first;
            prog.setUniformMat4("camera", getMatrix());
        }

    }
}



glm::vec3 CalcRightVector(const std::vector<float>& upVec, const std::vector<float>& frontVec) {
    glm::vec3 cameraUp = glm::vec3(upVec[0], upVec[1], upVec[2]);
    glm::vec3 cameraFront = glm::vec3(frontVec[0], frontVec[1], frontVec[2]);
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
    return right;
}

CameraControllerFPSStyle::CameraControllerFPSStyle(Camera& camera)
: _camera(camera)
, _pitch(0.0f)
, _yaw(0.0f)
, _moveSpeed(2.5f)
, _turnSpeed(10.0f) {
    _camera.setPosition(0.0f, 0.0f, 0.0f);
    _camera.setFront(1.0f, 0.0f, 0.0f);
}

void CameraControllerFPSStyle::goForward(float moveTime) {
    std::vector<float> frontVec = _camera.getFront();
    glm::vec3 cameraFront = glm::vec3(frontVec[0], frontVec[1], frontVec[2]);
    std::vector<float> posVec = _camera.getPosition();
    glm::vec3 cameraPos = glm::vec3(posVec[0], posVec[1], posVec[2]);
    cameraPos += moveTime * _moveSpeed * cameraFront;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraControllerFPSStyle::goBack(float moveTime) {
    std::vector<float> frontVec = _camera.getFront();
    glm::vec3 cameraFront = glm::vec3(frontVec[0], frontVec[1], frontVec[2]);
    std::vector<float> posVec = _camera.getPosition();
    glm::vec3 cameraPos = glm::vec3(posVec[0], posVec[1], posVec[2]);
    cameraPos -= moveTime * _moveSpeed * cameraFront;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraControllerFPSStyle::goLeft(float moveTime) {
    glm::vec3 cameraRight = CalcRightVector(_camera.getUp(), _camera.getFront());
    std::vector<float> posVec = _camera.getPosition();
    glm::vec3 cameraPos = glm::vec3(posVec[0], posVec[1], posVec[2]);
    cameraPos -= moveTime * _moveSpeed * cameraRight;
    setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}
void CameraControllerFPSStyle::goRight(float moveTime) {
    glm::vec3 cameraRight = CalcRightVector(_camera.getUp(), _camera.getFront());
    std::vector<float> posVec = _camera.getPosition();
    glm::vec3 cameraPos = glm::vec3(posVec[0], posVec[1], posVec[2]);
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
    _camera.setFront(front.x, front.y, front.z);

}
void CameraControllerFPSStyle::setPosition(float x, float y, float z) {
    _camera.setPosition(x, y, z);
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
