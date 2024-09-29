#include "shader/ColorGeometryPoint.h"
#include "Utils.h"


static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorGeometryPointShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorGeometryPointShader.fs");
    static const std::string GS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/glsl/ColorGeometryPointShader.gs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, GS_SHADER_STR);
    return prog;
}

const std::vector<ShaderAttribDescriptor> ColorGeometryPointVertex::DESCRIPTOR = {
    DESC("aPos",   0, ColorGeometryPointVertex, pos),
    DESC("aColor", 1, ColorGeometryPointVertex, color)
};


ColorGeometryPoint::ColorGeometryPoint()
: AbstractShader(GetShaderProg(), RenderDataMode::POINTS) {

}

ColorGeometryPoint::ColorGeometryPoint(const std::vector<ColorGeometryPointVertex>& points)
: AbstractShader(GetShaderProg(), RenderDataMode::POINTS)
, _points(points) {
    setVertexData(points, {});
}

void ColorGeometryPoint::setPoints(const std::vector<ColorGeometryPointVertex>& points) {
    _points = points;
    setVertexData(points, {});
}

void ColorGeometryPoint::updateUniformes() {

}
