#include "shader/ColorTexMulilight3DShader.h"
#include "shader/IncorporateColorTex3DShader.h"
#include "shader/Color3DShader.h"
#include "shader/ColorTex2DShader.h"
#include "shader/ColorTex3DShader.h"
#include "shader/ColorGeometryPointShader.h"
#include "shader/ColorTexcubeShader.h"
#include "math.h"

RenderShape cubeShape = {{
    {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f},   {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f},   {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},

    {{-0.5f, -0.5f, 0.5f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f},   {0.0f,  0.0f, 1.0f},   {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f},    {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f},    {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.5f},   {0.0f,  0.0f, 1.0f},   {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

    {{-0.5f, 0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.5f},   {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

    {{0.5f, 0.5f, -0.5f},   {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f},    {1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f},   {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f},   {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f},   {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

    {{0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f},   {0.0f, -1.0f,  0.0f},  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f},  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

    {{-0.5f, 0.5f, 0.5f},   {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f},    {0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f},   {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f},   {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f, -0.5f},  {0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.5f},   {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
}};

RenderShape tetrahedronShape = {{
    {{ 0.5f,  0.5f,  0.5f}, { 0.577f,  0.577f,  0.577f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {-0.577f,  0.577f, -0.577f}, {0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, {-0.577f, -0.577f,  0.577f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

    {{ 0.5f,  0.5f,  0.5f}, { 0.577f,  0.577f,  0.577f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, {-0.577f, -0.577f,  0.577f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.577f, -0.577f, -0.577f}, {0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}},

    {{ 0.5f,  0.5f,  0.5f}, { 0.577f,  0.577f,  0.577f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.577f, -0.577f, -0.577f}, {0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {-0.577f,  0.577f, -0.577f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

    {{-0.5f, -0.5f,  0.5f}, {-0.577f, -0.577f,  0.577f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {-0.577f,  0.577f, -0.577f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.577f, -0.577f, -0.577f}, {0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}},
}};

RenderShape rectShape = {
{
    {{-0.5f, -0.5f, 0.0f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}, {0.5f, 0.5f, 0.0f}},
    {{0.5f,  -0.5f, 0.0f}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 0.5f, 0.5f}},
    {{0.5f,  0.5f,  0.0f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}, {0.5f, 0.0f, 0.5f}},
    {{-0.5f, 0.5f,  0.0f}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}, {0.5f, 0.5f, 0.5f}},
}, 
{   0, 1, 2,
    0, 2, 3
}
};


std::vector<RenderShape::Vertex> CalcCircleShape(const Position &center, float radius, int numSegments) {
    const float step = 2 * M_PI / numSegments;
    std::vector<RenderShape::Vertex> vertices(numSegments);
    for (int i = 0; i < numSegments; ++i) {
        float angle = i * step;
        vertices[i] = {{cos(angle) * radius, sin(angle) * radius, 0}, {0, 0, -1}, {0, 0}, {cos(angle), cos(angle), sin(angle)}};
    }
    return vertices;
}

std::vector<unsigned int> CalcCircleShapeIndices(int numSegments) {
    std::vector<unsigned int> indices(numSegments * 3);
    for (int index = 0; index < numSegments; ++index) {
        indices[index * 3 + 0] = 0;
        indices[index * 3 + 1] = index;
        indices[index * 3 + 2] = index + 1;
    }
    return indices;
}

RenderShape circleShape(CalcCircleShape({0, 0, 0}, 1.f, 256), CalcCircleShapeIndices(256));

