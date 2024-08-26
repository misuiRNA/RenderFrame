#include "shader/Color3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/Color3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/Color3DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


Color3D::Color3D(bool isParallel)
: AbstractShader(GetShaderProg(), RenderDataMode::TRIANGLES)
, _shaderLight(isParallel)
, _size(1.0f, 1.0f, 1.0f) {

}

Color3D::operator const ShaderLight&() const {
    return _shaderLight;
}

void Color3D::setColor(const Color& color) {
    _shaderLight.setColor(color);
}

void Color3D::setSize(const Size3D& size) {
    _size = size;
}

void Color3D::setSpotFacor(float angle, float expandSmoothRatio) {
    _shaderLight.setSpotAngle(angle);
    _shaderLight.setSpotOuterRatio(expandSmoothRatio);
}

void Color3D::setPosition(const Position& pos) {
    _shaderLight.setPosition(pos);
}

void Color3D::setDirection(const Vector3D& dir) {
    _shaderLight.setSpotDirection(dir);
}

void Color3D::setReach(float distance) {
    _shaderLight.setReach(distance);
}

void Color3D::setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos", 0, Vertex, pos)
    };

    _renderData.setVertices(vertices, descriptor);

    if (!indices.empty()) {
        _renderData.setIndices(indices);
    }
}

const Position& Color3D::getPosition() const {
    return _shaderLight.getPosition();
}

Color Color3D::getColor() const {
    return _shaderLight.getColor();
}

bool Color3D::isParallel() const {
    return _shaderLight.isParallel();
}

void Color3D::updateUniformes() {
    const Color& orgColor = _shaderLight.getColor();
    _renderData.setUniform("color", orgColor.r, orgColor.g, orgColor.b, 1.0f);

    glm::mat4 model;
    const Position& pos = _shaderLight.getPosition();
    model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
    model = glm::scale(model, glm::vec3(_size.x, _size.y, _size.z));
    Matrix4X4 modelMatrix;
    memcpy(&modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
    _renderData.setUniform("modelMatrix", modelMatrix);
}
