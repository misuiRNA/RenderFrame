#include "model/Cuboid.h"
#include "model/IncorporateRectangle3D.h"
#include "model/LightSource.h"
#include "model/Rectangle2D.h"
#include "model/Rectangle3D.h"
#include "model/RichPoints.h"
#include "model/SkyBox.h"


// Cuboid
std::vector<Cuboid::Vertex> cubeVertices = {
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},  {0.0f,  0.0f, -1.0f}},
    {{0.5f, 0.5f, -0.5f},   {1.0f, 1.0f},  {0.0f,  0.0f, -1.0f}},
    {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f},  {0.0f,  0.0f, -1.0f}},
    {{0.5f, 0.5f, -0.5f},   {1.0f, 1.0f},  {0.0f,  0.0f, -1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},  {0.0f,  0.0f, -1.0f}},
    {{-0.5f, 0.5f, -0.5f},  {0.0f, 1.0f},  {0.0f,  0.0f, -1.0f}},

    {{-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f},  {0.0f,  0.0f, 1.0f}},
    {{0.5f, -0.5f, 0.5f},   {1.0f, 0.0f},  {0.0f,  0.0f, 1.0f}},
    {{0.5f, 0.5f, 0.5f},    {1.0f, 1.0f},  {0.0f,  0.0f, 1.0f}},
    {{0.5f, 0.5f, 0.5f},    {1.0f, 1.0f},  {0.0f,  0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.5f},   {0.0f, 1.0f},  {0.0f,  0.0f, 1.0f}},
    {{-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f},  {0.0f,  0.0f, 1.0f}},

    {{-0.5f, 0.5f, -0.5f},  {1.0f, 0.0f},  {-1.0f,  0.0f,  0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {0.0f, 1.0f},  {-1.0f,  0.0f,  0.0f}},
    {{-0.5f, 0.5f, 0.5f},   {1.0f, 1.0f},  {-1.0f,  0.0f,  0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {0.0f, 1.0f},  {-1.0f,  0.0f,  0.0f}},
    {{-0.5f, 0.5f, -0.5f},  {1.0f, 0.0f},  {-1.0f,  0.0f,  0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},  {-1.0f,  0.0f,  0.0f}},

    {{0.5f, 0.5f, -0.5f},   {1.0f, 0.0f},  {1.0f,  0.0f,  0.0f}},
    {{0.5f, 0.5f, 0.5f},    {1.0f, 1.0f},  {1.0f,  0.0f,  0.0f}},
    {{0.5f, -0.5f, 0.5f},   {0.0f, 1.0f},  {1.0f,  0.0f,  0.0f}},
    {{0.5f, -0.5f, 0.5f},   {0.0f, 1.0f},  {1.0f,  0.0f,  0.0f}},
    {{0.5f, -0.5f, -0.5f},  {0.0f, 0.0f},  {1.0f,  0.0f,  0.0f}},
    {{0.5f, 0.5f, -0.5f},   {1.0f, 0.0f},  {1.0f,  0.0f,  0.0f}},

    {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f},  {0.0f, -1.0f,  0.0f}},
    {{0.5f, -0.5f, 0.5f},   {1.0f, 1.0f},  {0.0f, -1.0f,  0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {0.0f, 1.0f},  {0.0f, -1.0f,  0.0f}},
    {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f},  {0.0f, -1.0f,  0.0f}},
    {{-0.5f, -0.5f, 0.5f},  {0.0f, 1.0f},  {0.0f, -1.0f,  0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},  {0.0f, -1.0f,  0.0f}},

    {{-0.5f, 0.5f, 0.5f},   {0.0f, 1.0f},  {0.0f,  1.0f,  0.0f}},
    {{0.5f, 0.5f, 0.5f},    {1.0f, 1.0f},  {0.0f,  1.0f,  0.0f}},
    {{0.5f, 0.5f, -0.5f},   {1.0f, 0.0f},  {0.0f,  1.0f,  0.0f}},
    {{0.5f, 0.5f, -0.5f},   {1.0f, 0.0f},  {0.0f,  1.0f,  0.0f}},
    {{-0.5f, 0.5f, -0.5f},  {0.0f, 0.0f},  {0.0f,  1.0f,  0.0f}},
    {{-0.5f, 0.5f, 0.5f},   {0.0f, 1.0f},  {0.0f,  1.0f,  0.0f}},
};


std::vector<Cuboid::Vertex> tetrahedronVertices = {
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, { 0.577f,  0.577f,  0.577f}},
    {{-0.5f,  0.5f, -0.5f}, {0.5f, 1.0f}, {-0.577f,  0.577f, -0.577f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {-0.577f, -0.577f,  0.577f}},

    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, { 0.577f,  0.577f,  0.577f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {-0.577f, -0.577f,  0.577f}},
    {{ 0.5f, -0.5f, -0.5f}, {0.5f, 1.0f}, { 0.577f, -0.577f, -0.577f}},

    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, { 0.577f,  0.577f,  0.577f}},
    {{ 0.5f, -0.5f, -0.5f}, {0.5f, 1.0f}, { 0.577f, -0.577f, -0.577f}},
    {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}, {-0.577f,  0.577f, -0.577f}},

    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {-0.577f, -0.577f,  0.577f}},
    {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}, {-0.577f,  0.577f, -0.577f}},
    {{ 0.5f, -0.5f, -0.5f}, {0.5f, 1.0f}, { 0.577f, -0.577f, -0.577f}},
};


std::vector<IncorporateRectangle3D::Vertex> rectVertices = {
    {{-0.5f, -0.5f, 0.0f},  {0.0f, 0.0f}},
    {{0.5f,  -0.5f, 0.0f},  {1.0f, 0.0f}},
    {{0.5f,  0.5f,  0.0f},  {1.0f, 1.0f}},
    {{-0.5f, 0.5f,  0.0f},  {0.0f, 1.0f}},
};
std::vector<unsigned int> rectIndices = {
    0, 1, 2,
    0, 2, 3
};


std::vector<LightSource::Vertex> cubeVertices_light = {
    {{-0.5f, -0.5f, -0.5f}},
    {{0.5f, 0.5f, -0.5f}},
    {{0.5f, -0.5f, -0.5f}},
    {{0.5f, 0.5f, -0.5f}},
    {{-0.5f, -0.5f, -0.5f}},
    {{-0.5f, 0.5f, -0.5f}},

    {{-0.5f, -0.5f, 0.5f}},
    {{0.5f, -0.5f, 0.5f}},
    {{0.5f, 0.5f, 0.5f}},
    {{0.5f, 0.5f, 0.5f}},
    {{-0.5f, 0.5f, 0.5f}},
    {{-0.5f, -0.5f, 0.5f}},

    {{-0.5f, 0.5f, -0.5f}},
    {{-0.5f, -0.5f, 0.5f}},
    {{-0.5f, 0.5f, 0.5f}},
    {{-0.5f, -0.5f, 0.5f}},
    {{-0.5f, 0.5f, -0.5f}},
    {{-0.5f, -0.5f, -0.5f}},

    {{0.5f, 0.5f, -0.5f}},
    {{0.5f, 0.5f, 0.5f}},
    {{0.5f, -0.5f, 0.5f}},
    {{0.5f, -0.5f, 0.5f}},
    {{0.5f, -0.5f, -0.5f}},
    {{0.5f, 0.5f, -0.5f}},

    {{0.5f, -0.5f, -0.5f}},
    {{0.5f, -0.5f, 0.5f}},
    {{-0.5f, -0.5f, 0.5f}},
    {{0.5f, -0.5f, -0.5f}},
    {{-0.5f, -0.5f, 0.5f}},
    {{-0.5f, -0.5f, -0.5f}},

    {{-0.5f, 0.5f, 0.5f}},
    {{0.5f, 0.5f, 0.5f}},
    {{0.5f, 0.5f, -0.5f}},
    {{0.5f, 0.5f, -0.5f}},
    {{-0.5f, 0.5f, -0.5f}},
    {{-0.5f, 0.5f, 0.5f}},
};


std::vector<LightSource::Vertex> tetrahedronVertices_light = {
    {{ 0.5f,  0.5f,  0.5f}},
    {{-0.5f,  0.5f, -0.5f}},
    {{-0.5f, -0.5f,  0.5f}},
    {{ 0.5f,  0.5f,  0.5f}},
    {{-0.5f, -0.5f,  0.5f}},
    {{ 0.5f, -0.5f, -0.5f}},
    {{ 0.5f,  0.5f,  0.5f}},
    {{ 0.5f, -0.5f, -0.5f}},
    {{-0.5f,  0.5f, -0.5f}},
    {{-0.5f, -0.5f,  0.5f}},
    {{-0.5f,  0.5f, -0.5f}},
    {{ 0.5f, -0.5f, -0.5f}},
};


std::vector<Rectangle2D::Vertex> rectVertices_rect2D = {
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f,  0.0f}, {1.0f, 0.0f}},
    {{0.5f,  0.5f,  0.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f,  0.0f}, {0.0f, 1.0f}},
};
std::vector<unsigned int> rectIndices_rect2D = {
    0, 1, 2,
    0, 2, 3
};


std::vector<Rectangle3D::Vertex> rectVertices_rect3D = {
    {{-0.5f, -0.5f, 0.0f},  {0.0f, 0.0f}},
    {{0.5f,  -0.5f, 0.0f},  {1.0f, 0.0f}},
    {{0.5f,  0.5f,  0.0f},  {1.0f, 1.0f}},
    {{-0.5f, 0.5f,  0.0f},  {0.0f, 1.0f}},
};
std::vector<unsigned int> rectIndices_rect3D = {
    0, 1, 2,
    0, 2, 3
};


std::vector<RichPoints::Vertex> points_richPoints = {
    {{-0.5f, 0.5f,  0.0}, {1.0f, 0.0f, 0.0f}},
    {{0.5f,  0.5f,  0.0}, {0.0f, 1.0f, 0.0f}},
    {{0.5f,  -0.5f, 0.0}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f, 0.0}, {0.0f, 0.0f, 0.0f}}
};


std::vector<SkyBox::Vertex> cubeVertices_skybox = {
    {{-1.0f,  1.0f, -1.0f}},
    {{-1.0f, -1.0f, -1.0f}},
    {{1.0f, -1.0f, -1.0f}},
    {{1.0f, -1.0f, -1.0f}},
    {{1.0f,  1.0f, -1.0f}},
    {{-1.0f,  1.0f, -1.0f}},

    {{-1.0f, -1.0f,  1.0f}},
    {{-1.0f, -1.0f, -1.0f}},
    {{-1.0f,  1.0f, -1.0f}},
    {{-1.0f,  1.0f, -1.0f}},
    {{-1.0f,  1.0f,  1.0f}},
    {{-1.0f, -1.0f,  1.0f}},

    {{1.0f, -1.0f, -1.0f}},
    {{1.0f, -1.0f,  1.0f}},
    {{1.0f,  1.0f,  1.0f}},
    {{1.0f,  1.0f,  1.0f}},
    {{1.0f,  1.0f, -1.0f}},
    {{1.0f, -1.0f, -1.0f}},

    {{-1.0f, -1.0f,  1.0f}},
    {{-1.0f,  1.0f,  1.0f}},
    {{1.0f,  1.0f,  1.0f}},
    {{1.0f,  1.0f,  1.0f}},
    {{1.0f, -1.0f,  1.0f}},
    {{-1.0f, -1.0f,  1.0f}},

    {{-1.0f,  1.0f, -1.0f}},
    {{1.0f,  1.0f, -1.0f}},
    {{1.0f,  1.0f,  1.0f}},
    {{1.0f,  1.0f,  1.0f}},
    {{-1.0f,  1.0f,  1.0f}},
    {{-1.0f,  1.0f, -1.0f}},

    {{-1.0f, -1.0f, -1.0f}},
    {{-1.0f, -1.0f,  1.0f}},
    {{1.0f, -1.0f, -1.0f}},
    {{1.0f, -1.0f, -1.0f}},
    {{-1.0f, -1.0f,  1.0f}},
    {{1.0f, -1.0f,  1.0f}}
};


std::vector<SkyBox::Vertex> tetrahedronVertices_skybox = {
    {{ 1.0f,  1.0f,  1.0f}},
    {{-1.0f,  1.0f, -1.0f}},
    {{-1.0f, -1.0f,  1.0f}},
    {{ 1.0f,  1.0f,  1.0f}},
    {{-1.0f, -1.0f,  1.0f}},
    {{ 1.0f, -1.0f, -1.0f}},
    {{ 1.0f,  1.0f,  1.0f}},
    {{ 1.0f, -1.0f, -1.0f}},
    {{-1.0f,  1.0f, -1.0f}},
    {{-1.0f, -1.0f,  1.0f}},
    {{-1.0f,  1.0f, -1.0f}},
    {{ 1.0f, -1.0f, -1.0f}},
};