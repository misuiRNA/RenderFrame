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

void Camera::setFront(float pitch, float yaw)
{
    // TODO: 优化相机状态建模  当前建模 pitch=0, yaw=0 时对应 方向为(1.0, 0.0, 0.0)
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
    setFront(front.x, front.y, front.z);
}

void Camera::setFov(float fov)
{
    if(fov <= 1.0f) {
        fov = 1.0f;
    } else if(fov >= 45.0f) {
        fov = 45.0f;
    }
    _fov = fov;
}

const std::vector<float> Camera::getFront() const
{
    return std::vector<float>{_frontX, _frontY, _frontZ};
}

const std::vector<float> Camera::getPosition() const
{
    return std::vector<float>{_x, _y, _z};
}

const std::vector<float> Camera::getRight() const
{
    glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f,  1.0f);    // Up Vector
    glm::vec3 cameraFront = glm::vec3(_frontX, _frontY, _frontZ);
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
    return std::vector<float>{right.x, right.y, right.z};
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

void Camera::enabel()
{
    for (auto itr : ShaderProgram::getAllShaderProg())
    {
        if (itr.first) {
            ShaderProgram& prog = *itr.first;
            prog.setUniformMat4("camera", getMatrix());
        }

    }
}
