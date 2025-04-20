#include "base/Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static Vector3D Normalize(const Vector3D& vec) {
    glm::vec3 normalGlmVec = glm::normalize(glm::vec3(vec.x, vec.y, vec.z));
    return Vector3D(normalGlmVec.x, normalGlmVec.y, normalGlmVec.z);
}

Transform::Transform(const Vector3D& up, const Vector3D& front, const Position& pos, const Size3D& size)
: _pos(pos)
, _up(Normalize(up))
, _front(Normalize(front))
, _size(size) {

}

Transform& Transform::setPosition(const Position& pos) {
    _pos = pos;
    attitudeChanged();
    return *this;
}

Transform& Transform::setFront(const Vector3D& front) {
    _front = Normalize(front);
    attitudeChanged();
    return *this;
}

Transform& Transform::setUp(const Vector3D& up) {
    _up = Normalize(up);
    attitudeChanged();
    return *this;
}

Transform& Transform::setSize(const Size3D& size) {
    _size = size;
    attitudeChanged();
    return *this;
}

const Matrix4X4& Transform::getMatrix() const {
    return _matrix;
}

const Position& Transform::getPosition() const {
    return _pos;
}

const Vector3D& Transform::getFront() const {
    return _front;
}

const Vector3D& Transform::getUp() const {
    return _up;
}

Vector3D Transform::getRight() const {
    glm::vec3 cameraUp = glm::vec3(_up.x, _up.y, _up.z);
    glm::vec3 cameraFront = glm::vec3(_front.x, _front.y, _front.z);
    glm::vec3 normalRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    Vector3D rightVec(normalRight.x, normalRight.y, normalRight.z);
    return rightVec;
}

const Size3D& Transform::getSize() const {
    return _size;
}

static glm::mat4 GetAttitudeMatrix(const Vector3D& up, const Vector3D& front) {
    // TODO: 优化, 需要保证上向量和前向量不平行, 否则旋转矩阵无效
    glm::vec3 normalUp = glm::vec3(up.x, up.y, up.z);
    glm::vec3 normalFront = glm::normalize(glm::vec3(front.x, front.y, front.z));
    glm::vec3 normalRight = glm::normalize(glm::cross(normalUp, normalFront));
    glm::mat4 attitudeMatrix = glm::mat4(1.0f);
    attitudeMatrix[0] = glm::vec4(normalRight, 0.0f);
    attitudeMatrix[1] = glm::vec4(normalUp,    0.0f);
    attitudeMatrix[2] = glm::vec4(normalFront, 0.0f);
    attitudeMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return attitudeMatrix;
}

void Transform::attitudeChanged() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));
    model = model * GetAttitudeMatrix(_up, _front);
    model = glm::scale(model, glm::vec3(_size.x, _size.y, _size.z));

    memcpy(&_matrix, glm::value_ptr(model), sizeof(glm::mat4));
}
