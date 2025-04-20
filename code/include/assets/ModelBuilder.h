#ifndef _HEAD_FLAG_MODELBUILDER_TRACE_H
#define _HEAD_FLAG_MODELBUILDER_TRACE_H

#include <vector>
#include "RenderEngine.h"

struct ModelBuilder {
    static RenderShape Build3DModelData(const std::vector<Position>& skeletonPos, const std::vector<RenderShape::Vertex>& crossSectionVertices, float scale);
};

#endif