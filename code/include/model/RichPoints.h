#ifndef _HEAD_FLAG_MODEL_RICHPOINTS_H
#define _HEAD_FLAG_MODEL_RICHPOINTS_H

#include "model/AbstractDrawObject.h"

struct RichPoints : AbstractDrawObject {
    RichPoints();
    RichPoints(const std::vector<Position>& points);

    void setPoints(const std::vector<Position>& points);

private:
    void updateUniformes() override;
    void updateRenderData() override;

private:
    std::vector<Position> _points;
};

#endif    // _HEAD_FLAG_MODEL_RICHPOINTS_H
