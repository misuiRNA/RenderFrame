#include "ShaderCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderCamera::ShaderCamera()
: _trans({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f})
, _fov(45.0f) {
    updateMatrix();
}

ShaderCamera::ShaderCamera(const ShaderCamera& oth)
: _trans(oth._trans)
, _fov(oth._fov) {
    updateMatrix();
}

void ShaderCamera::setPosition(const Position& pos) {
    _trans.setPosition(pos);
    updateMatrix();
}

void ShaderCamera::move(const Vector3D& vec) {
    setPosition(_trans.getPosition() + vec);
}

void ShaderCamera::setFront(const Vector3D& front) {
    _trans.setFront(front);
    updateMatrix();
}

void ShaderCamera::setFov(float fov) {
    if(fov <= 1.0f) {
        fov = 1.0f;
    } else if(fov >= 80.0f) {
        fov = 80.0f;
    }
    _fov = fov;
    updateMatrix();
}

const Transform& ShaderCamera::getTransform() const {
    return _trans;
}

const Position& ShaderCamera::getPosition() const {
    return _trans.getPosition();
}

const Matrix4X4& ShaderCamera::getMatrix() const {
    return _matrix;
}

void ShaderCamera::updateMatrix() {
    glm::mat4 view;

    const Position& pos = _trans.getPosition();
    const Vector3D& front = _trans.getFront();
    const Vector3D& up = _trans.getUp();
    view = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
                       glm::vec3(pos.x + front.x, pos.y + front.y, pos.z + front.z),
                       glm::vec3(up.x, up.y, up.z));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(_fov), 1.0f * 800 / 600, 0.1f, 100.0f);
    view = projection * view;

    memcpy(&_matrix, glm::value_ptr(view), sizeof(glm::mat4));
}
