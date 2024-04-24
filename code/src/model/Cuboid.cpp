#include "model/Cuboid.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"

Cubiod::Cubiod(float size_x, float size_y, float size_z)
: AbstractModel(ShaderProgram::getCuboidShaderProg())
, _pos({0.0f, 0.0f, 0.0f})
, _size(size_x, size_y, size_z)
, _scaleRatio(1.0f)
, _rotation(0.0f)
, _imageCount(0)
, _rotationAxis({0.0f, 0.0f, 1.0f})
, _color(1.0f, 1.0f, 1.0f) {
    // TODO: 优化, 优化uniform的初始化方式, 不强制要求_renderData的客户手动初始化 uniform, 容易遗漏初始化代码
    _renderData.setTexture("texture1", 0);
    _renderData.setTexture("texture2", 0);
    _renderData.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
}

void Cubiod::setPosition(const Position& pos) {
    _pos = pos;
}

void Cubiod::setSize(const Size3D& size) {
    _size = size;
}

void Cubiod::setScaleRatio(float scaleRatio) {
     _scaleRatio = scaleRatio;
}

void Cubiod::setRotation(float rotation) {
    _rotation = rotation;
}

void Cubiod::setRotationAxis(const Vector3D& axis) {
    _rotationAxis = axis;
}

void Cubiod::setColor(const Color& color) {
    _color = color;
}

// TODO: 优化, 减少重复加载
void Cubiod::addImage(const Image& image) {
    _imageCount += 1;
    if (_imageCount == 1) {
        _renderData.setTexture("texture1", image.getTexture());
    } else if (_imageCount == 2) {
        _renderData.setTexture("texture2", image.getTexture());
    }
}

void Cubiod::setMaterial(const ShaderMaterial& material) {
    _renderData.setUniform("material", material);
}

void Cubiod::updateUniformes() {
    _renderData.setUniform("imageEnable", _imageCount);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));
    if (_rotationAxis.x != 0.0f || _rotationAxis.y != 0.0f || _rotationAxis.z != 0.0f) {
        model = glm::rotate(model, _rotation, glm::vec3(_rotationAxis.x, _rotationAxis.y, _rotationAxis.z));
    }
    model = glm::scale(model, glm::vec3(_scaleRatio * _size.x, _scaleRatio * _size.y, _scaleRatio * _size.z));

    _renderData.setUniformMat4("modelMatrix", glm::value_ptr(model));
}

void Cubiod::updateRenderData() {
    _renderData.setVertices<Vector3D>("aPos", {
        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},

        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},

        {-0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},

        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},

        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, -0.5f},

        {-0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},
    });
    _renderData.setVertices<Vector2D>("aTexCoord", {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        {1.0f, 0.0f},

        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        {1.0f, 0.0f},

        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f},

        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f},
    });
    _renderData.setVertices<Vector3D>("aNormal", {
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},

        {0.0f,  0.0f, 1.0f},
        {0.0f,  0.0f, 1.0f},
        {0.0f,  0.0f, 1.0f},
        {0.0f,  0.0f, 1.0f},
        {0.0f,  0.0f, 1.0f},
        {0.0f,  0.0f, 1.0f},

        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},

        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},

        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},

        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
    });
}
