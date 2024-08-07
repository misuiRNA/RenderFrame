#include "model/RichPoints.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/RichPointsShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/RichPointsShader.fs");
    static const std::string GS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/RichPointsShader.gs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, GS_SHADER_STR);
    return prog;
}


RichPoints::RichPoints()
: AbstractDrawObject(GetShaderProg(), RenderDataMode::POINTS) {

}

RichPoints::RichPoints(const std::vector<Vertex>& points)
: AbstractDrawObject(GetShaderProg(), RenderDataMode::POINTS)
, _points(points) {
    setVertexData(points);
}

void RichPoints::setPoints(const std::vector<Vertex>& points) {
    _points = points;
    setVertexData(points);
}

void RichPoints::setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",   0, Vertex, pos),
        DESC("aColor", 1, Vertex, color)
    };
    _renderData.setVertices(_points, descriptor);

    if (!indices.empty()) {
        _renderData.setIndices(indices);
    }
}

void RichPoints::updateUniformes() {

}
