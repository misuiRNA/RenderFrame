#include "model/Rectangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"

Rectangle::Rectangle(float width, float height)
: AbstractModel(ShaderProgram::getRectShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _front(0.0f, 0.0f, 1.0f)
, _width(width)
, _height(height)
, _scaleRatio(1.0f)
, _imageEnable(false) {

}

void Rectangle::setPosition(const Position& pos) {
    _pos = pos;
}

void Rectangle::setFront(const Vector3D& front) {
    _front = front;
}

void Rectangle::setSize(float width, float height) {
    _width = width;
    _height = height;
}

void Rectangle::setScaleRatio(float scaleRatio) {
    _scaleRatio = scaleRatio;
}

// TODO: 优化, 支持贴图设置不同环绕方式
void Rectangle::setImage(const Image& image) {
    _renderData.setTexture("texture1", image.getTexture(TextureWrapMode::ClampToEdge));
    _imageEnable = true;
}

void Rectangle::setColor(const Color& color) {
    _color = color;
}

void Rectangle::updateUniformes() {
    _renderData.setUniform("imageEnable", _imageEnable);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));

    // TODO: 优化, 需要保证上向量和前向量不平行, 否则旋转矩阵无效
    Vector3D up(0.0f, 0.0f, 1.0f);
    glm::vec3 normalUp = glm::vec3(up.x, up.y, up.z);
    glm::vec3 normalFront = glm::normalize(glm::vec3(_front.x, _front.y, _front.z));
    glm::vec3 normalRight = glm::normalize(glm::cross(normalUp, normalFront));
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix[0] = glm::vec4(normalRight, 0.0f);
    rotationMatrix[1] = glm::vec4(normalUp,    0.0f);
    rotationMatrix[2] = glm::vec4(normalFront, 0.0f);
    rotationMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    model = model * rotationMatrix;

    model = glm::scale(model, glm::vec3(_scaleRatio * _width, _scaleRatio * _height, 1.0f));
    _renderData.setUniformMat4("modelMatrix", glm::value_ptr(model));
}

void Rectangle::updateRenderData() {
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
