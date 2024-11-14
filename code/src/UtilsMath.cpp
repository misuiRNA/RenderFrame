#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float MathUtils::AngleCos(float angle) {
    return glm::cos(glm::radians(angle));
}

RenderShape LineUtils::LineToLane(const std::vector<Position>& linePoints, float width) {
    std::vector<Position> orgPoints(linePoints);
    LineUtils::SmoothNoisyPoints(orgPoints, width * 3.0f, 10.0f);

    std::vector<Position> lLine;
    std::vector<Position> rLine;
    LineUtils::ExpandLineToAngleBisector(orgPoints, width, lLine, rLine);

    printf("===> orgPoints-size: %lu/%lu, lLine-size: %lu, rLine-size=%lu\n", orgPoints.size(), linePoints.size(), lLine.size(), rLine.size());

    RenderShape res;
    res.vertices.reserve(rLine.size() + lLine.size());
    for (int rIndex = 0, lIndex = 0; rIndex < rLine.size() && lIndex < lLine.size(); ++rIndex, ++lIndex) {
        res.vertices.emplace_back(RenderShape::Vertex{rLine[rIndex], Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 0.0f, 0.0f}});
        res.vertices.emplace_back(RenderShape::Vertex{lLine[lIndex], Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 0.0f, 0.0f}});
    }

    unsigned long halfVetSize = res.vertices.size() / 2;
    res.indices.reserve(halfVetSize * 6);
    for (int count = 0; count < halfVetSize - 1; ++count) {
        int index = count * 2;
        res.indices.emplace_back(index);
        res.indices.emplace_back(index + 3);
        res.indices.emplace_back(index + 1);

        res.indices.emplace_back(index);
        res.indices.emplace_back(index + 2);
        res.indices.emplace_back(index + 3);
    }

    return res;
}
