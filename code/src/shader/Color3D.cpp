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
, _pos(0.0f, 0.0f, 0.0f)
, _size(1.0f, 1.0f, 1.0f)
, _color(1.0f, 1.0f, 1.0f) {

}

void Color3D::setColor(const Color& color) {
    _color = color;
}

void Color3D::setSize(const Size3D& size) {
    _size = size;
}

void Color3D::setPosition(const Position& pos) {
    _pos = pos;
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

void Color3D::setVertexData(const RenderShape& shape) {
    std::vector<Vertex> vertices;
    vertices.reserve(shape.vertices.size());
    for (const auto& vertex : shape.vertices) {
        vertices.emplace_back(vertex.pos);
    }
    setVertexData(vertices, shape.indices);
}

void Color3D::updateUniformes() {
    const Color& orgColor = _color;
    _renderData.setUniform("color", orgColor.r, orgColor.g, orgColor.b, 1.0f);

    glm::mat4 model;
    const Position& pos = _pos;
    model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
    model = glm::scale(model, glm::vec3(_size.x, _size.y, _size.z));
    Matrix4X4 modelMatrix;
    memcpy(&modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
    _renderData.setUniform("modelMatrix", modelMatrix);
}