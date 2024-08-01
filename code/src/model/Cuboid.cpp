#include "model/Cuboid.h"
#include "ShaderProgram.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Cuboid.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Cuboid.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


Cuboid::Cuboid(const Size3D& size)
: AbstractDrawObject(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, size)
, _imageCount(0)
, _color(1.0f, 1.0f, 1.0f) {
    _renderData.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
}

Cuboid::Cuboid(const Cuboid& oth)
: AbstractDrawObject(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl(oth._attitudeCtrl)
, _imageCount(oth._imageCount)
, _color(oth._color) {
    _renderData.setUniform("material", ShaderMaterial(_color * 0.2f, _color * 0.8f, _color * 1.0f));
}

void Cuboid::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void Cuboid::setSize(const Size3D& size) {
    _attitudeCtrl.setSize(size);
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

// TODO: 优化, 重新设置顶点数据后需要清除旧顶点VBO
void Cuboid::setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",      0, Vertex, pos),
        DESC("aTexCoord", 1, Vertex, texCoord),
        DESC("aNormal",   2, Vertex, normal)
    };
    _renderData.setVertices(vertices, descriptor);

    if (!indices.empty()) {
        _renderData.setIndices(indices);
    }
}

void Cuboid::updateUniformes() {
    _renderData.setUniform("imageEnable", _imageCount);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _renderData.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}
