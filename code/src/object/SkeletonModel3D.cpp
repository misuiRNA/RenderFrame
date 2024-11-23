#include "object/SkeletonModel3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


SkeletonModel3D::SkeletonModel3D()
: _shader({1.0f, 1.0f, 1.0f}) {
    // _shader.setDrawMode(RenderDataMode::LINE_STRIP);
    // _shader.setPosition({-2.0f, 3.0f, 5.0f});
    _shader.setColor(Color(0.0f, 0.0f, 0.0f));
    _shader.getAttituedeCtrl().setFront({0.0f, 0.0f, 1.0f}).setUp({0.0f, 1.0f, 0.0f});

    _crossSection = {
        {Position{0.0f, -1.0f, 0.0f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{1.0f, 0.0f, 0.0f}},
        // {Position{0.0f, 0.0f, 0.01f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 1.0f, 0.0f}},
        {Position{0.0f, 1.0f, 0.0f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 0.0f, 1.0f}}
    };
}

void SkeletonModel3D::show() {
    _shader.show();
}

static glm::mat4 GetRotationMatrix(const Vector3D& up, const Vector3D& front) {
    // 目标坐标系为右手坐标系
    glm::vec3 normalUp = glm::vec3(up.x, up.y, up.z);
    glm::vec3 normalFront = glm::normalize(glm::vec3(front.x, front.y, front.z));
    glm::vec3 normalRight = glm::normalize(glm::cross(normalUp, normalFront));
    glm::mat4 attitudeMatrix = glm::mat4(1.0f);
    attitudeMatrix[0] = glm::vec4(normalRight, 0.0f);
    attitudeMatrix[1] = glm::vec4(normalUp,    0.0f);
    attitudeMatrix[2] = glm::vec4(normalFront, 0.0f);
    attitudeMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return attitudeMatrix;
}


static std::vector<int> BuildSideFaceIndices(int corssVertSize) {
    std::vector<int> sideFaceIndices;
    int N = corssVertSize;
    for (int vetIdx = 0; vetIdx < N; ++vetIdx) {
        int num0 = vetIdx;
        int num1 = num0 == N - 1 ? 0 : num0 + 1;
        int numN = num0 + N;
        int numN1 = num0 == N - 1 ? N : numN + 1;

        sideFaceIndices.emplace_back(num0);
        sideFaceIndices.emplace_back(num1);
        sideFaceIndices.emplace_back(numN1);

        sideFaceIndices.emplace_back(num0);
        sideFaceIndices.emplace_back(numN1);
        sideFaceIndices.emplace_back(numN);
    }
    return sideFaceIndices;
}

static RenderShape Build3DModelData(const std::vector<Position>& skeletonPos, const std::vector<RenderShape::Vertex>& crossSectionVertices, float scale) {
    std::vector<Vector3D> normals = LineUtils::CalcExpandVectorByAngleBisector(skeletonPos, 1.0f);

    std::vector<RenderShape::Vertex> vertices;
    vertices.reserve(crossSectionVertices.size() * skeletonPos.size());
    for (int index = 0; index < skeletonPos.size(); ++index) {
        glm::mat4 transMat;
        transMat = glm::translate(transMat, glm::vec3(skeletonPos[index].x, skeletonPos[index].y, skeletonPos[index].z));
        transMat = transMat * GetRotationMatrix(normals[index], {0.0f, 0.0f, 1.0f});
        transMat = glm::scale(transMat, glm::vec3(scale, scale, scale));

        for (int corssIdx = 0; corssIdx < crossSectionVertices.size(); ++corssIdx) {
            RenderShape::Vertex vert = crossSectionVertices[corssIdx];
            glm::vec4 targetPos = transMat * glm::vec4{vert.pos.x, vert.pos.y, vert.pos.z, 1.0f};
            vert.pos = Position{targetPos.x, targetPos.y, targetPos.z};
            vertices.emplace_back(vert);
        }
    }

    std::vector<int> sideFaceProtoIndices = BuildSideFaceIndices(crossSectionVertices.size());
    int sideFaceProtoIndicesSize = sideFaceProtoIndices.size();
    std::vector<unsigned int> sideFaceIndices;
    sideFaceIndices.reserve(skeletonPos.size() * sideFaceProtoIndicesSize);
    for (int posIdx = 0; posIdx < skeletonPos.size() - 1; ++posIdx) {
        int offset = posIdx * crossSectionVertices.size();
        for (int vetIdx = 0; vetIdx < sideFaceProtoIndicesSize; ++vetIdx) {
            sideFaceIndices.emplace_back(offset + sideFaceProtoIndices[vetIdx]);
        }
    }

    RenderShape res(vertices, sideFaceIndices);
    printf("===> skeletonPos-size: %lu, vertices-size: %lu, indices-size=%lu\n", skeletonPos.size(), res.vertices.size(), res.indices.size());
    return res;
}

void SkeletonModel3D::append(const Position& pos) {
    _MidLinePosits.emplace_back(pos);
    RenderShape laneShape = Build3DModelData(_MidLinePosits, _crossSection, 0.2f);
    _shader.setVertexData(laneShape);
}

void SkeletonModel3D::append(const std::vector<Position>& points) {
    _MidLinePosits.insert(_MidLinePosits.end(), points.begin(), points.end());
    RenderShape laneShape = Build3DModelData(_MidLinePosits, _crossSection, 0.3f);
    _shader.setVertexData(laneShape);
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
