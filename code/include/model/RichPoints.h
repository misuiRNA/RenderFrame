#ifndef _HEAD_FLAG_MODEL_RICHPOINTS_H
#define _HEAD_FLAG_MODEL_RICHPOINTS_H

#include "model/AbstractDrawObject.h"


struct RichPointsNode {
    Position pos;
    Color color;
};

struct RichPoints : AbstractDrawObject {
    RichPoints();
    RichPoints(const std::vector<RichPointsNode>& points);

    void setPoints(const std::vector<RichPointsNode>& points);

private:
    void updateUniformes() override;
    void updateRenderData() override;

private:
    std::vector<RichPointsNode> _points;
};

#endif    // _HEAD_FLAG_MODEL_RICHPOINTS_H
