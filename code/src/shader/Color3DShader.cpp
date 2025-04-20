#include "shader/Color3DShader.h"
#include "assets/UtilsFileSys.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
#include "base/Utils.h"


const ShaderAttribDescriptor Color3DVertex::DESCRIPTOR = DESC_NEW(Color3DVertex,
    ITEM(Color3DVertex, 0, "aPos", pos)
);

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/Color3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/Color3DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


Color3DShader::Color3DShader(bool isParallel)
: AbstractShader(GetShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _size(1.0f, 1.0f, 1.0f)
, _color(1.0f, 1.0f, 1.0f) {

}

void Color3DShader::setColor(const Color& color) {
    _color = color;
}

void Color3DShader::setSize(const Size3D& size) {
    _size = size;
}

void Color3DShader::setPosition(const Position& pos) {
    _pos = pos;
}

void Color3DShader::updateUniformes() {
    const Color& orgColor = _color;
    _engine.setUniform("color", orgColor.r, orgColor.g, orgColor.b, 1.0f);

    glm::mat4 model;
    const Position& pos = _pos;
    model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
    model = glm::scale(model, glm::vec3(_size.x, _size.y, _size.z));
    Matrix4X4 modelMatrix;
    memcpy(&modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
    _engine.setUniform("modelMatrix", modelMatrix);
}
