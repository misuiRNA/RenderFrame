#include "ShaderCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderCamera::ShaderCamera()
: _pos(0.0f, 0.0f, 0.0f)
, _front(0.0f, 0.0f, 1.0f)
, _fov(45.0f) {
    updateMatrix();
}

void ShaderCamera::setPosition(const Position& pos) {
    _pos = pos;
    updateMatrix();
}

void ShaderCamera::setFront(const Vector3D& front) {
    _front = front;
    updateMatrix();
}

void ShaderCamera::setFov(float fov) {
    if(fov <= 1.0f) {
        fov = 1.0f;
    } else if(fov >= 45.0f) {
        fov = 45.0f;
    }
    _fov = fov;
}

const Vector3D& ShaderCamera::getFront() const {
    return _front;
}

const Position& ShaderCamera::getPosition() const {
    return _pos;
}

const Vector3D& ShaderCamera::getUp() const {
    static Vector3D upVector { 0.0f, 0.0f,  1.0f };
    return upVector;
}

const float* ShaderCamera::getMatrix() const {
    return _matrix;
}

void ShaderCamera::updateMatrix() {
    glm::mat4 view;

    view = glm::lookAt(glm::vec3(_pos.x, _pos.y, _pos.z),
                       glm::vec3(_pos.x + _front.x, _pos.y + _front.y, _pos.z + _front.z),
                       glm::vec3(0.0f, 0.0f, 1.0f));    // Up Vector

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(_fov), 1.0f * 800 / 600, 0.1f, 100.0f);
    view = projection * view;

    memcpy(_matrix, glm::value_ptr(view), sizeof(glm::mat4));
}
