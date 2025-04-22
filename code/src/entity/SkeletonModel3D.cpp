#include "entity/SkeletonModel3D.h"
#include "assets/ModelBuilder.h"


SkeletonModel3D::SkeletonModel3D()
: _shader({1.0f, 1.0f, 1.0f}) {
    // _shader.setDrawMode(RenderDataMode::LINE_STRIP);
    // _shader.setPosition({-2.0f, 3.0f, 5.0f});
    _shader.setColor(Color(0.0f, 0.0f, 0.0f));
    _shader.getTransform().setFront({0.0f, 0.0f, 1.0f}).setUp({0.0f, 1.0f, 0.0f});

    _crossSection = {
        {Position{0.0f, -1.0f, 0.0f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{1.0f, 0.0f, 0.0f}},
        // {Position{0.0f, 0.0f, 0.01f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 1.0f, 0.0f}},
        {Position{0.0f, 1.0f, 0.0f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 0.0f, 1.0f}}
    };
}

void SkeletonModel3D::show() {
    _shader.show();
}

void SkeletonModel3D::append(const Position& pos) {
    _MidLinePosits.emplace_back(pos);
    RenderShape laneShape = ModelBuilder::Build3DModelData(_MidLinePosits, _crossSection, 0.2f);
    _shader.setVertexData(RenderShapeAdapter<ColorTex3DShader>(laneShape));
}

void SkeletonModel3D::append(const std::vector<Position>& points) {
    _MidLinePosits.insert(_MidLinePosits.end(), points.begin(), points.end());
    RenderShape laneShape = ModelBuilder::Build3DModelData(_MidLinePosits, _crossSection, 0.3f);
    _shader.setVertexData(RenderShapeAdapter<ColorTex3DShader>(laneShape));
}

void SkeletonModel3D::setCrossSection(const std::vector<RenderShape::Vertex>& crossSection) {
    _crossSection = crossSection;
}

void SkeletonModel3D::setPosition(const Position& pos) {
    _shader.setPosition(pos);
}

Position SkeletonModel3D::getPosition() {
    return _MidLinePosits.empty() ? Position{0.0f, 0.0f, 0.0f} : _MidLinePosits.back();
}
