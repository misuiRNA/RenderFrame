#include "object/Trace.h"


Trace::Trace()
: _shader({1.0f, 1.0f, 1.0f}) {
    // _shader.setDrawMode(RenderDataMode::LINE_STRIP);
    // _shader.setPosition({-2.0f, 3.0f, 5.0f});
    _shader.setColor(Color(1.0f, 1.0f, 0.0f));
    _shader.getAttituedeCtrl().setFront({0.0f, 0.0f, 1.0f}).setUp({0.0f, 1.0f, 0.0f});
}

void Trace::show() {
    _shader.show();
}

void Trace::append(const Position& pos) {
    _MidLinePosits.emplace_back(pos);
    RenderShape laneShape = LineUtils::LineToLane(_MidLinePosits, 0.2f);
    _shader.setVertexData(laneShape);
}

void Trace::append(const std::vector<Position>& points) {
    _MidLinePosits.insert(_MidLinePosits.end(), points.begin(), points.end());
    RenderShape laneShape = LineUtils::LineToLane(_MidLinePosits, 0.3f);
    _shader.setVertexData(laneShape);
}

Position Trace::getPosition() {
    return _MidLinePosits.empty() ? Position{0.0f, 0.0f, 0.0f} : _MidLinePosits.back();
}
