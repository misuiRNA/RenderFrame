#include "model/Model3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model3DLoader.h"

Model3D::Model3D(std::string const& path)
: AbstractModel(ShaderProgram::getMeshShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _front(1.0f, 0.0f, 0.0f)
, _up(0.0f, 0.0f, 1.0f)
, _scaleRatio(1.0f)
, _modelPath(path) {

}

void Model3D::setPosition(const Position& pos) {
    _pos = pos;
}

void Model3D::setFront(const Vector3D& front) {
    _front = front;
}

void Model3D::setScale(float scale) {
    _scaleRatio = scale;
}

void Model3D::updateUniformes() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));

    glm::vec3 normalUp = glm::vec3(_up.x, _up.y, _up.z);
    glm::vec3 normalFront = glm::normalize(glm::vec3(_front.x, _front.y, _front.z));
    glm::vec3 normalRight = glm::normalize(glm::cross(normalUp, normalFront));
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix[0] = glm::vec4(normalRight, 0.0f);
    rotationMatrix[1] = glm::vec4(normalUp,    0.0f);
    rotationMatrix[2] = glm::vec4(normalFront, 0.0f);
    rotationMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    model = model * rotationMatrix;
    model = glm::scale(model, glm::vec3(_scaleRatio, _scaleRatio, _scaleRatio));

    _renderData.setUniformMat4("model", glm::value_ptr(model));
}

void Model3D::updateRenderData() {
    Model3DLoader loader;
    const std::vector<Mesh>& meshes = loader.loadModel(_modelPath);
    _renderData.setMeshes(meshes);
}