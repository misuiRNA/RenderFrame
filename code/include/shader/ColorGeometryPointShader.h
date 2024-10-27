#ifndef _HEAD_FLAG_MODEL_RICHPOINTS_H
#define _HEAD_FLAG_MODEL_RICHPOINTS_H

#include "render/AbstractShader.h"

struct ColorGeometryPointVertex {
    ColorGeometryPointVertex(const RenderShape::Vertex& data) : pos(data.pos), color(data.color) { }
    Position pos;
    Color color;

    static const ShaderAttribDescriptor DESCRIPTOR;
};


struct ColorGeometryPointShader : AbstractShader<ColorGeometryPointVertex> {
    ColorGeometryPointShader();
    ColorGeometryPointShader(const std::vector<ColorGeometryPointVertex>& points);

    void setPoints(const std::vector<ColorGeometryPointVertex>& points);

private:
    void updateUniformes() override;

private:
    std::vector<ColorGeometryPointVertex> _points;
};

#endif    // _HEAD_FLAG_MODEL_RICHPOINTS_H
