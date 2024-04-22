#include "RenderData.h"
#include "glad/glad.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


RenderData::RenderData(ShaderProgram& prog)
: _prog(prog)
, _VAOId(0)
, _vertexCount(0)
, _indexCount(0) {

}

RenderData::~RenderData() {
    if (_VAOId != 0) {
        // TODO: delete _needFreeVAOSelf, 临时方案
        if (_needFreeVAOSelf) {
            glDeleteVertexArrays(1, &_VAOId);
        }
    }
}

void RenderData::setVertices(unsigned int index, unsigned int vertexSize, const std::vector<float>& vertices) {
    if (_VAOId == 0) {
        glGenVertexArrays(1, &_VAOId);
    }
    glBindVertexArray(_VAOId);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(index, vertexSize, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(index);

    _needFreeVAOSelf = true;

    // glBindVertexArray(0);
    // TODO should release VBO manual ?
}

void RenderData::setVertices(const std::string& name, unsigned int vertexSize, const std::vector<float>& vertices) {
    if (!_prog.checkVertice(name)) {
        std::cout << "Failed to set attribute! name not found: " << name << std::endl;
        return;
    }
    unsigned int index = _prog.getVerticeSlotId(name);
    setVertices(index, vertexSize, vertices);
    _vertexCount = vertices.size();
}

void RenderData::setIndices(const std::vector<unsigned int>& indices) {
    if (_VAOId == 0)
    {
        glGenVertexArrays(1, &_VAOId);
    }
    glBindVertexArray(_VAOId);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // glBindVertexArray(0);
    // TODO should release EBO manual ?
    _indexCount = indices.size();
}

void RenderData::setTexture(const std::string& name, unsigned int textureId) {
    // if (textureId == 0) {
    //     std::cout << "Failed to set texture! textureId is invalid: " << name  << " value=" << textureId << std::endl;
    //     return;
    // }

    if (_textureMap.find(name) == _textureMap.end()) {
        int newSlotIndex = (int)_textureMap.size();
        _textureMap[name] = newSlotIndex;
    }
    int slotIndex = _textureMap[name];
    setUniform(name, slotIndex);

    std::function<void(void)> func = [slotIndex, textureId](void) -> void {
            glActiveTexture(GL_TEXTURE0 + slotIndex);
            glBindTexture(GL_TEXTURE_2D, textureId);
    };
    setTextureFunc(name, func);
}

void RenderData::setUniform(const std::string& name, int value) {
    std::function<void(void)> func = [this, name, value]() -> void {
        _prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float value) {
    std::function<void(void)> func = [this, name, value]() -> void {
        _prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float v1, float v2, float v3) {
    std::function<void(void)> func = [this, name, v1, v2, v3]() -> void {
        _prog.setUniform(name, v1, v2, v3);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float v1, float v2, float v3, float v4) {
    std::function<void(void)> func = [this, name, v1, v2, v3, v4]() -> void {
        _prog.setUniform(name, v1, v2, v3, v4);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniformMat4(const std::string& name, const float* mat)
{
    glm::mat4 matrix = glm::make_mat4(mat);
    std::function<void(void)> func = [this, name, matrix]() -> void {
        _prog.setUniformMat4(name, glm::value_ptr(matrix));
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, const XYZ& value)
{
    std::function<void(void)> func = [this, name, value]() -> void {
        _prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, const Color& color)
{
    std::function<void(void)> func = [this, name, color]() -> void {
        _prog.setUniform(name, color);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, const ShaderMaterial& material) {
    setTexture(name + ".diffuseTexture", material.diffuseTexture);
    setTexture(name + ".specularTexture", material.specularTexture);
    setUniform(name + ".ambient", material.ambient);
    setUniform(name + ".diffuse", material.diffuse);
    setUniform(name + ".specular", material.specular);
    setUniform(name + ".shininess", material.shininess);
}

void RenderData::setUniformFunc(const std::string& name, const std::function<void(void)>& func) {
    _uniformFunctions[name] = func;
}

void RenderData::setTextureFunc(const std::string& name, const std::function<void(void)>& func) {
    _textureFunctions[name] = func;
}

void RenderData::setChildren(const std::vector<RenderData>& children) {
    _children = children;
}

RenderData RenderData::genChild() {
    return RenderData(_prog);
}

void RenderData::useUniforms() {
    for (auto& pair : _uniformFunctions) {
        auto& setUniformFunc = pair.second;
        setUniformFunc();
    }
}

void RenderData::useTextures() {
    for (auto& pair : _textureFunctions) {
        auto& setTextureFunc = pair.second;
        setTextureFunc();
    }
}

void RenderData::drawAttributes() {
    if (_VAOId == 0) {
        // std::cout << "RenderData: draw failed, VAOId is 0!" << std::endl;
        return;
    }

    glBindVertexArray(_VAOId);
    if (_indexCount > 0) {
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
    }

    // always good practice to set everything back to defaults once configured.
    // glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void RenderData::draw() {
    useTextures();
    _prog.enable();
    useUniforms();

    drawAttributes();
    for (RenderData& child : _children) {
        child.draw();
    }
}
