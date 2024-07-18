#include "model/RichPoints.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/RichPointsShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/RichPointsShader.fs");
    static const std::string GS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/RichPointsShader.gs");
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",   0, RichPointsNode, pos),
        DESC("aColor", 1, RichPointsNode, color)
    };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, GS_SHADER_STR, descriptor);
    return prog;
}


RichPoints::RichPoints()
: AbstractDrawObject(GetShaderProg(), RenderDataMode::POINTS) {

}

RichPoints::RichPoints(const std::vector<RichPointsNode>& points)
: AbstractDrawObject(GetShaderProg(), RenderDataMode::POINTS)
, _points(points) {

}

void RichPoints::setPoints(const std::vector<RichPointsNode>& points) {

}

void RichPoints::updateUniformes() {

}

void RichPoints::updateRenderData() {
    _renderData.setVertices(_points);
}
