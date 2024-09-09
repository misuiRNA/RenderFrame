#include "shader/ColorGeometryPoint.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorGeometryPointShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorGeometryPointShader.fs");
    static const std::string GS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorGeometryPointShader.gs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, GS_SHADER_STR);
    return prog;
}


ColorGeometryPoint::ColorGeometryPoint()
: AbstractShader(GetShaderProg(), RenderDataMode::POINTS) {

}

ColorGeometryPoint::ColorGeometryPoint(const std::vector<Vertex>& points)
: AbstractShader(GetShaderProg(), RenderDataMode::POINTS)
, _points(points) {
    setVertexData(points);
}

void ColorGeometryPoint::setPoints(const std::vector<Vertex>& points) {
    _points = points;
    setVertexData(points);
}

void ColorGeometryPoint::setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",   0, Vertex, pos),
        DESC("aColor", 1, Vertex, color)
    };
    _renderData.setVertices(vertices, descriptor);

    if (!indices.empty()) {
        _renderData.setIndices(indices);
    }
}

void ColorGeometryPoint::setVertexData(const RenderShape& shape) {
    std::vector<Vertex> vertices;
    vertices.reserve(shape.vertices.size());
    for (const auto& vertex : shape.vertices) {
        vertices.emplace_back(vertex.pos, vertex.color);
    }
    setVertexData(vertices, shape.indices);
}

void ColorGeometryPoint::updateUniformes() {

}
