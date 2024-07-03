#include "model/Rectangle2D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Utils.h"

struct Rectangle2DVertex {
    Vector3D pos;
    Vector2D texCoord;
};

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle2DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle2DShader.fs");
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC_NEW("aPos",      0, Rectangle2DVertex, pos),
        DESC_NEW("aTexCoord", 1, Rectangle2DVertex, texCoord),
    };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, descriptor);
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
    std::vector<Rectangle2DVertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f,  0.0f}, {1.0f, 0.0f}},
        {{0.5f,  0.5f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f,  0.0f}, {0.0f, 1.0f}},
    };
    _renderData.setVertices(vertices);

    _renderData.setIndices({
        0, 1, 2,
        0, 2, 3
    });
}
