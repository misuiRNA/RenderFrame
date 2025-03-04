#include "shader/ColorGeometryPointShader.h"
#include "Utils.h"


const ShaderAttribDescriptor ColorGeometryPointVertex::DESCRIPTOR = DESC_NEW(ColorGeometryPointVertex,
    ITEM(ColorGeometryPointVertex, 0, "aPos",   pos),
    ITEM(ColorGeometryPointVertex, 1, "aColor", color)
);

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorGeometryPointShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorGeometryPointShader.fs");
    static const std::string GS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/ColorGeometryPointShader.gs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, GS_SHADER_STR);
    return prog;
}


ColorGeometryPointShader::ColorGeometryPointShader()
: AbstractShader(GetShaderProg()) {
    setDrawMode(RenderDataMode::POINTS);
}

ColorGeometryPointShader::ColorGeometryPointShader(const std::vector<ColorGeometryPointVertex>& points)
: AbstractShader(GetShaderProg())
, _points(points) {
    setDrawMode(RenderDataMode::POINTS);
    setVertexData(points, {});
}

void ColorGeometryPointShader::setPoints(const std::vector<ColorGeometryPointVertex>& points) {
    _points = points;
    setVertexData(points, {});
}

void ColorGeometryPointShader::updateUniformes() {

}
