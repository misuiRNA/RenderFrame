#include "model/Rectangle2D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Utils.h"

static ShaderProgram& GetShaderProg() {
    static const std::string MODEL_NAME = "Rectangle2D";
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle2DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle2DShader.fs");
    static const std::map<std::string, int> ATTRIBUTE_NAME_MAP = {
        {"aPos"     , 0},
        {"aTexCoord", 1},
    };

    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, ATTRIBUTE_NAME_MAP);
    return prog;
}

Rectangle2D::Rectangle2D(float width, float height)
: AbstractDrawObject(GetShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _width(width)
, _height(height)
, _textureEnable(false) {

}

void Rectangle2D::setPosition(const Position& pos) {
    _pos = pos;
}

void Rectangle2D::setImage(const AbstractImage& image) {
    _renderData.setTexture("texture1", image.getTexture(ImageWrapMode::ClampToEdge));
    _textureEnable = true;
}

void Rectangle2D::setColor(const Color& color) {
    _color = color;
}

void Rectangle2D::updateUniformes() {
    _renderData.setUniform("imageEnable", _textureEnable);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(_width, _height, 1.0f));

    _renderData.setUniformMat4("modelMatrix", glm::value_ptr(model));
}

void Rectangle2D::updateRenderData() {
    _renderData.setVertices<Vector3D>("aPos", {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
    });
    _renderData.setVertices<Vector2D>("aTexCoord", {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
    });
    _renderData.setIndices({
        0, 1, 2,
        0, 2, 3
    });
}
