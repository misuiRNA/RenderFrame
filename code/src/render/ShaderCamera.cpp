#include "ShaderCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderCamera::ShaderCamera()
: _pos(0.0f, 0.0f, 0.0f)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f})
, _fov(45.0f) {
    updateMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this]() { updateMatrix(); });
}

ShaderCamera::ShaderCamera(const ShaderCamera& oth)
: _pos(oth._pos)
, _attitudeCtrl(oth._attitudeCtrl.getUp(), oth._attitudeCtrl.getFront())
, _fov(oth._fov) {
    updateMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this]() { updateMatrix(); });
}

void ShaderCamera::setPosition(const Position& pos) {
    _pos = pos;
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

Attitude3DController& ShaderCamera::getAttituedeCtrl() {
    return _attitudeCtrl;
}

const Position& ShaderCamera::getPosition() const {
    return _pos;
}

const float* ShaderCamera::getMatrix() const {
    return _matrix;
}

void ShaderCamera::updateMatrix() {
    glm::mat4 view;

    const Vector3D& front = _attitudeCtrl.getFront();
    const Vector3D& up = _attitudeCtrl.getUp();
    view = glm::lookAt(glm::vec3(_pos.x, _pos.y, _pos.z),
                       glm::vec3(_pos.x + front.x, _pos.y + front.y, _pos.z + front.z),
                       glm::vec3(up.x, up.y, up.z));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(_fov), 1.0f * 800 / 600, 0.1f, 100.0f);
    view = projection * view;

    memcpy(_matrix, glm::value_ptr(view), sizeof(glm::mat4));
}
