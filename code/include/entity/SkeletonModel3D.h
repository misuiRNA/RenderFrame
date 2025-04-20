#ifndef _HEAD_FLAG_SKELETONMODEL3D_TRACE_H
#define _HEAD_FLAG_SKELETONMODEL3D_TRACE_H
#include "shader/ColorTex3DShader.h"

struct SkeletonModel3D {
    SkeletonModel3D();
    void show();
    void append(const Position& pos);
    void append(const std::vector<Position>& points);
    void setCrossSection(const std::vector<RenderShape::Vertex>& crossSection);
    void setPosition(const Position& pos);
    Position getPosition();

private:
    ColorTex3DShader _shader;
    std::vector<Position> _MidLinePosits;
    std::vector<RenderShape::Vertex> _crossSection;
};

#endif