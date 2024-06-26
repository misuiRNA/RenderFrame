#include "Attitude3DController.h"


static Vector3D Normalize(const Vector3D& vec) {
    glm::vec3 normalGlmVec = glm::normalize(glm::vec3(vec.x, vec.y, vec.z));
    return Vector3D(normalGlmVec.x, normalGlmVec.y, normalGlmVec.z);
}

Attitude3DController::Attitude3DController(const Vector3D& up, const Vector3D& front) : _up(Normalize(up)), _front(Normalize(front)) {

}

Attitude3DController& Attitude3DController::setFront(const Vector3D& front) {
    _front = Normalize(front);
    attitudeChanged();
    return *this;
}

Attitude3DController& Attitude3DController::setUp(const Vector3D& up) {
    _up = Normalize(up);
    attitudeChanged();
    return *this;
}

glm::mat4 Attitude3DController::getAttitudeMatrix() const {
    // TODO: 优化, 需要保证上向量和前向量不平行, 否则旋转矩阵无效
    const Vector3D& up = _up;
    glm::vec3 normalUp = glm::vec3(up.x, up.y, up.z);
    glm::vec3 normalFront = glm::normalize(glm::vec3(_front.x, _front.y, _front.z));
    glm::vec3 normalRight = glm::normalize(glm::cross(normalUp, normalFront));
    glm::mat4 attitudeMatrix = glm::mat4(1.0f);
    attitudeMatrix[0] = glm::vec4(normalRight, 0.0f);
    attitudeMatrix[1] = glm::vec4(normalUp,    0.0f);
    attitudeMatrix[2] = glm::vec4(normalFront, 0.0f);
    attitudeMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return attitudeMatrix;
}

const Vector3D& Attitude3DController::getFront() const {
    return _front;
}

const Vector3D& Attitude3DController::getUp() const {
    return _up;
}

Vector3D Attitude3DController::getRight() const {
    glm::vec3 cameraUp = glm::vec3(_up.x, _up.y, _up.z);
    glm::vec3 cameraFront = glm::vec3(_front.x, _front.y, _front.z);
    glm::vec3 normalRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    Vector3D rightVec(normalRight.x, normalRight.y, normalRight.z);
    return rightVec;
}

void Attitude3DController::addOnAttitudeChangedListener(const std::function<void()>& listener) {
    _onAttitudeChangedListener.emplace_back(listener);
}

void Attitude3DController::attitudeChanged() const {
    for (auto& listener : _onAttitudeChangedListener) {
        listener();
    }
}
