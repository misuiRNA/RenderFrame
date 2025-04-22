#ifndef _HEAD_FLAG_MODEL_RICHPOINTS_H
#define _HEAD_FLAG_MODEL_RICHPOINTS_H

#include "render/AbstractShader.h"

struct ColorGeometryPointVertex {
    Position pos;
    Color color;
};


struct ColorGeometryPointShader : AbstractShader {
     using Vertex = ColorGeometryPointVertex;

    ColorGeometryPointShader();
    ColorGeometryPointShader(const std::vector<ColorGeometryPointVertex>& points);
    void setPoints(const std::vector<ColorGeometryPointVertex>& points);

private:
    void updateUniformes() override;

private:
    std::vector<ColorGeometryPointVertex> _points;

public:
    static const ShaderAttribDescriptor DESCRIPTOR;
    static ColorGeometryPointVertex BuildVertex(const RenderShape::Vertex& data) {
        ColorGeometryPointVertex vertex;
        vertex.pos = data.pos;
        vertex.color = data.color;
        return vertex;
    }
};

#endif    // _HEAD_FLAG_MODEL_RICHPOINTS_H
