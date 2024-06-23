#include "model/Cuboid.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"

Cuboid::Cuboid(const Size3D& size)
: AbstractDrawObject(ShaderProgram::GetCuboidShaderProg())
, _pos({0.0f, 0.0f, 0.0f})
, _size(size)
, _attitudeCtrl({0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f})
, _imageCount(0)
, _color(1.0f, 1.0f, 1.0f) {
    _renderData.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
    _attitudeCtrl.addOnAttitudeChangedListener([this]() { updateModelMatrix(); });
}

Cuboid::Cuboid(const Cuboid& oth)
: AbstractDrawObject(ShaderProgram::GetCuboidShaderProg())
, _pos(oth._pos)
, _size(oth._size)
, _attitudeCtrl(oth._attitudeCtrl.getUp(), oth._attitudeCtrl.getFront())
, _imageCount(oth._imageCount)
, _color(oth._color) {
    _renderData.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
    _attitudeCtrl.addOnAttitudeChangedListener([this]() { updateModelMatrix(); });
}

void Cuboid::setPosition(const Position& pos) {
    _pos = pos;
    updateModelMatrix();
}

void Cuboid::setSize(const Size3D& size) {
    _size = size;
    updateModelMatrix();
}

void Cuboid::setColor(const Color& color) {
    _color = color;
}

void Cuboid::addImage(const AbstractImage& image) {
    _imageCount += 1;
    if (_imageCount == 1) {
        _renderData.setTexture("texture1", image.getTexture(ImageWrapMode::Repeat));
    } else if (_imageCount == 2) {
        _renderData.setTexture("texture2", image.getTexture(ImageWrapMode::Repeat));
    }
}

Attitude3DController& Cuboid::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void Cuboid::setMaterial(const ShaderMaterial& material) {
    _renderData.setUniform("material", material);
}

void Cuboid::updateModelMatrix() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));
    glm::mat4 attitudeMatrix = _attitudeCtrl.getAttitudeMatrix();
    model = model * attitudeMatrix;
    model = glm::scale(model, glm::vec3(_size.x, _size.y, _size.z));

    memcpy(_modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
}

void Cuboid::updateUniformes() {
    _renderData.setUniform("imageEnable", _imageCount);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _renderData.setUniformMat4("modelMatrix", _modelMatrix);
}

void Cuboid::updateRenderData() {
    _renderData.setVertices<Vector3D>("aPos", {
        {-0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},

        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},

        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},

        {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},

        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, -0.5f},

        {-0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, 0.5f},
    });
    _renderData.setVertices<Vector2D>("aTexCoord", {
        {0.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f},
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
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

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
