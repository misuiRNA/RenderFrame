#ifndef _HEAD_FLAG_MODEL_RICHPOINTS_H
#define _HEAD_FLAG_MODEL_RICHPOINTS_H

#include "shader/AbstractShader.h"

struct ColorGeometryPointVertex {
    ColorGeometryPointVertex(const RenderShape::Vertex& data) : pos(data.pos), color(data.color) { }
    Position pos;
    Color color;

    static const ShaderAttribDescriptor DESCRIPTOR;
};


struct ColorGeometryPoint : AbstractShader<ColorGeometryPointVertex> {
    ColorGeometryPoint();
    ColorGeometryPoint(const std::vector<ColorGeometryPointVertex>& points);

    void setPoints(const std::vector<ColorGeometryPointVertex>& points);

private:
    void updateUniformes() override;

private:
    std::vector<ColorGeometryPointVertex> _points;
};

#endif    // _HEAD_FLAG_MODEL_RICHPOINTS_H
