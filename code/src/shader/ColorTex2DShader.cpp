#include "shader/ColorTex2DShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "base/Utils.h"
#include "assets/UtilsFileSys.h"


const ShaderAttribDescriptor ColorTex2DShader::DESCRIPTOR = DESC_NEW(ColorTex2DVertex,
    ITEM(ColorTex2DVertex, 0, "aPos",      pos),
    ITEM(ColorTex2DVertex, 1, "aTexCoord", texCoord),
);

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorTex2DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorTex2DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


ColorTex2DShader::ColorTex2DShader(float width, float height)
: AbstractShader(GetShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _width(width)
, _height(height)
, _textureEnable(false) {

}

void ColorTex2DShader::setPosition(const Position& pos) {
    _pos = pos;
}

void ColorTex2DShader::setImage(const AbstractImage& image) {
    _engine.setTexture("texture1", image.getTexture(ImageWrapMode::ClampToEdge));
    _textureEnable = true;
}

void ColorTex2DShader::setColor(const Color& color) {
    _color = color;
}

void ColorTex2DShader::updateUniformes() {
    _engine.setUniform("imageEnable", _textureEnable);
    _engine.setUniform("color", _color.r, _color.g, _color.b, 1.0f);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(_width, _height, 1.0f));
    Matrix4X4 modelMatrix;
    memcpy(&modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
    _engine.setUniform("modelMatrix", modelMatrix);
}
