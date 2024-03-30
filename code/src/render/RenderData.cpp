#include "RenderData.h"
#include "glad/glad.h"
#include <iostream>


RenderData::RenderData() : RenderData({}, {}) {

}

RenderData::RenderData(const std::map<std::string, int>& attrNameMap, const std::map<std::string, int>& textureSlotNameMap)
: _vertexCount(0)
, _indexCount(0)
, _attrNameMap(attrNameMap)
, _textureSlotNameMap(textureSlotNameMap) {
    glGenVertexArrays(1, &_VAOId);
    // 保证未设置纹理的 RenderData 不会使用其他 RenderData 的纹理
    for (auto& pair : _textureSlotNameMap) {
        _textureMap[pair.second] = 0;
    }
}

RenderData::~RenderData() {
    glDeleteVertexArrays(1, &_VAOId);
}

void RenderData::setVertices(unsigned int index, unsigned int vertexSize, const std::vector<float>& vertices) {
    glBindVertexArray(_VAOId);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(index, vertexSize, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(index);

    // TODO should release VBO manual ?
    _vertexCount = vertices.size();
}

void RenderData::setVertices(const std::string& name, unsigned int vertexSize, const std::vector<float>& vertices) {
    if (_attrNameMap.find(name) == _attrNameMap.end()) {
        std::cout << "Failed to set attribute! name not found: " << name << std::endl;
        return;
    }
    setVertices(_attrNameMap.at(name), vertexSize, vertices);
}

void RenderData::setIndices(const std::vector<unsigned int>& indices) {
    glBindVertexArray(_VAOId);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // TODO should release EBO manual ?
    _indexCount = indices.size();
}

void RenderData::setTexture(unsigned int slotIndex, unsigned int width, unsigned int height, const unsigned char* imageData, unsigned int format) {
    if (!imageData) {
        std::cout << "Failed to set texture! imageData is null " << std::endl;
        return;
    }

    glActiveTexture(GL_TEXTURE0 + slotIndex);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    _textureMap[slotIndex] = texture;
}

// TODO 优化, 用其他方式携带format信息不要传参
void RenderData::setTexture(const std::string& name, unsigned int width, unsigned int height, const unsigned char* imageData, unsigned int format) {
    if (_textureSlotNameMap.find(name) == _textureSlotNameMap.end()) {
        std::cout << "Failed to set texture! name not found: " << name << std::endl;
        return;
    }
    setTexture(_textureSlotNameMap.at(name), width, height, imageData, format);
}

void RenderData::useTextures() {
    for (auto& entity : _textureMap) {
        int slotIndex = entity.first;
        unsigned int textureId = entity.second;
        glActiveTexture(GL_TEXTURE0 + slotIndex);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
}

void RenderData::draw() {
    glBindVertexArray(_VAOId);
    if (_indexCount > 0) {
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
    }
}
