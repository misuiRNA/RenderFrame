#include "RenderData.h"
#include "glad/glad.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


static auto VAODeleter = [](unsigned int* VAOPtr) {
    if (*VAOPtr != 0) {
        glDeleteVertexArrays(1, VAOPtr);
        *VAOPtr = 0;
        // delete VBO
        // delete EBO
    }
};

RenderData::RenderData(ShaderProgram& prog)
: _prog(prog)
, _VAOId(0)
, _VAOHolder(&_VAOId, VAODeleter)
, _vertexCount(0)
, _indexCount(0) {

}

// remind: 浅拷贝VAO, 拷贝构造对象共用VAO
RenderData::RenderData(const RenderData& oth)
: _prog(oth._prog)
, _VAOId(oth._VAOId)
, _VAOHolder(oth._VAOHolder)
, _vertexCount(oth._vertexCount)
, _indexCount(oth._indexCount)
, _textureMap(oth._textureMap)
, _uniformFunctions(oth._uniformFunctions) {

}

RenderData::RenderData(RenderData&& oth) noexcept
: _prog(oth._prog)
, _VAOId(oth._VAOId)
, _VAOHolder(std::move(oth._VAOHolder))
, _vertexCount(oth._vertexCount)
, _indexCount(oth._indexCount)
, _textureMap(std::move(oth._textureMap))
, _uniformFunctions(std::move(oth._uniformFunctions)) {
    oth._VAOId = 0;
    oth._vertexCount = 0;
    oth._indexCount = 0;
    oth._VAOHolder.reset();
}

RenderData::~RenderData() {
    // remind: VAO 等 gl 资源释放交给 _VAOHolder 管理
    _textureMap.clear();
    _uniformFunctions.clear();
}

RenderData& RenderData::operator=(RenderData&& oth) noexcept {
    if (this != &oth) {
        _prog = oth._prog;
        _VAOId = oth._VAOId;
        _vertexCount = oth._vertexCount;
        _indexCount = oth._indexCount;
        _textureMap = std::move(oth._textureMap);
        _uniformFunctions = std::move(oth._uniformFunctions);

        oth._VAOId = 0;
        oth._vertexCount = 0;
        oth._indexCount = 0;
    }
    return *this;
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

    // TODO should release VBO manual ?
    glBindVertexArray(0);
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

    // TODO should release EBO manual ?
    glBindVertexArray(0);
    _indexCount = indices.size();
}

void RenderData::setTexture(const std::string& name, unsigned int textureId) {
    // if (textureId == 0) {
    //     std::cout << "Failed to set texture! textureId is invalid: " << name  << " value=" << textureId << std::endl;
    //     return;
    // }
    _textureMap[name] = textureId;
}

void RenderData::setUniform(const std::string& name, int value) {
    std::function<void(ShaderProgram& prog)> func = [name, value](ShaderProgram& prog) -> void {
        prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float value) {
    std::function<void(ShaderProgram& prog)> func = [name, value](ShaderProgram& prog) -> void {
        prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float v1, float v2, float v3) {
    std::function<void(ShaderProgram& prog)> func = [name, v1, v2, v3](ShaderProgram& prog) -> void {
        prog.setUniform(name, v1, v2, v3);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float v1, float v2, float v3, float v4) {
    std::function<void(ShaderProgram& prog)> func = [name, v1, v2, v3, v4](ShaderProgram& prog) -> void {
        prog.setUniform(name, v1, v2, v3, v4);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniformMat4(const std::string& name, const float* mat)
{
    glm::mat4 matrix = glm::make_mat4(mat);
    std::function<void(ShaderProgram& prog)> func = [name, matrix](ShaderProgram& prog) -> void {
        prog.setUniformMat4(name, glm::value_ptr(matrix));
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, const XYZ& value)
{
    std::function<void(ShaderProgram& prog)> func = [name, value](ShaderProgram& prog) -> void {
        prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, const Color& color)
{
    std::function<void(ShaderProgram& prog)> func = [name, color](ShaderProgram& prog) -> void {
        prog.setUniform(name, color);
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

void RenderData::setUniformFunc(const std::string& name, const std::function<void(ShaderProgram& prog)>& func) {
    _uniformFunctions[name] = func;
}

ShaderProgram& RenderData::getShaderProgram() const {
    return _prog;
}

void RenderData::useUniforms() {
    for (auto& pair : _uniformFunctions) {
        auto& setUniformFunc = pair.second;
        setUniformFunc(_prog);
    }
}

void RenderData::useTextures() {
    int textureSlotIdx = 0;
    for (auto& itr : _textureMap) {
        const std::string& name = itr.first;
        unsigned int textureId = itr.second;

        _prog.setUniform(name, textureSlotIdx);
        glActiveTexture(GL_TEXTURE0 + textureSlotIdx);
        glBindTexture(GL_TEXTURE_2D, textureId);
        textureSlotIdx++;
    }
}

void RenderData::resetTextures() {
    int textureSlotIdx = 0;
    for (auto& itr : _textureMap) {
        const std::string& name = itr.first;
        unsigned int textureId = itr.second;

        _prog.setUniform(name, 0);
        glActiveTexture(GL_TEXTURE0 + textureSlotIdx);
        glBindTexture(GL_TEXTURE_2D, 0);
        textureSlotIdx++;
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
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void RenderData::draw() {
    _prog.enable();

    useTextures();
    useUniforms();
    drawAttributes();
    resetTextures();
}
