#include "shader/ColorTexcube.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorTexcubeShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorTexcubeShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


ColorTexcube::ColorTexcube()
: AbstractShader(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}) {

}

void ColorTexcube::setImage(const AbstractImage& image) {
    _renderData.setTexture("skybox", image.getTexture(ImageWrapMode::ClampToEdge));
}

// TODO: 优化, 天空盒子实现方式依赖外部设置中心点位置, 需要优化
void ColorTexcube::setCenter(const Position& center) {
    _attitudeCtrl.setPosition(center);
}

void ColorTexcube::setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos", 0, Vertex, pos),
    };

    _renderData.setVertices(vertices, descriptor);

    if (!indices.empty()) {
        _renderData.setIndices(indices);
    }
}

void ColorTexcube::setVertexData(const RenderShape& shape) {
    std::vector<Vertex> vertices;
    vertices.reserve(shape.vertices.size());
    for (const auto& vertex : shape.vertices) {
        vertices.emplace_back(vertex.pos);
    }
    setVertexData(vertices, shape.indices);
}

void ColorTexcube::updateUniformes() {
    _renderData.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}
