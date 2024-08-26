#ifndef _HEAD_FLAG_MODEL_RICHPOINTS_H
#define _HEAD_FLAG_MODEL_RICHPOINTS_H

#include "shader/AbstractShader.h"


struct ColorGeometryPoint : AbstractShader {
    struct Vertex {
        Position pos;
        Color color;
    };

    ColorGeometryPoint();
    ColorGeometryPoint(const std::vector<Vertex>& points);

    void setPoints(const std::vector<Vertex>& points);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});

private:
    void updateUniformes() override;

private:
    std::vector<Vertex> _points;
};

#endif    // _HEAD_FLAG_MODEL_RICHPOINTS_H
