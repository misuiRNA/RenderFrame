#ifndef _HEAD_FLAG_LANE_TRACE_H
#define _HEAD_FLAG_LANE_TRACE_H
#include "shader/ColorTex3DShader.h"

struct Trace {
    Trace();
    void show();
    void append(const Position& pos);
    Position getPosition();

private:
    ColorTex3DShader _shader;
    std::vector<Position> _MidLinePosits;
};

#endif