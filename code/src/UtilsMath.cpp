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

    std::vector<Position> lanePoints = rLine;
    lanePoints.insert(lanePoints.end(), lLine.begin(), lLine.end());

    int orgSize = rLine.size();
    std::vector<unsigned int> laneIndices;
    for (int index = 1; index < orgSize; ++index) {
        laneIndices.emplace_back(index - 1);
        laneIndices.emplace_back(index + orgSize);
        laneIndices.emplace_back(index + orgSize - 1);

        laneIndices.emplace_back(index - 1);
        laneIndices.emplace_back(index);
        laneIndices.emplace_back(index + orgSize);
    }

    std::function<RenderShape::Vertex(const Position&)> pos2VetCvt = [](const Position& pos) -> RenderShape::Vertex { return {pos}; };
    return RenderShape(ConvertList(lanePoints, pos2VetCvt), laneIndices);
}
