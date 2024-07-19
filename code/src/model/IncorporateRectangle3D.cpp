#include "model/IncorporateRectangle3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Utils.h"

struct Instance3DVertex {
    Position pos;
    Vector2D texcoord;
};

// TODO: 优化, 1.shader字符串编译时确定，不读取文件；2.返回的路径位置应为可执行文件位置，而不是执行命令的位置 考虑使用 std::filesystem
static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Instanced3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Instanced3DShader.fs");
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",      0, Instance3DVertex, pos),
        DESC("aTexCoord", 1, Instance3DVertex, texcoord),
    };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, descriptor);
    return prog;
}


IncorporateRectangle3D::IncorporateRectangle3D(const Size3D& size)
: AbstractDrawObject(GetShaderProg(), RenderDataMode::TRIANGLES)
, _pos(0.0f, 0.0f, 0.0f)
, _size(size)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f})
, _textureEnable(false) {
    updateModelMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this](){ updateModelMatrix(); });
}

IncorporateRectangle3D::IncorporateRectangle3D(const IncorporateRectangle3D& oth)
: AbstractDrawObject(oth)
, _pos(oth._pos)
, _size(oth._size)
, _attitudeCtrl(oth._attitudeCtrl.getUp(), oth._attitudeCtrl.getFront())
, _textureEnable(oth._textureEnable) {
    updateModelMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this](){ updateModelMatrix(); });
}

void IncorporateRectangle3D::setPosition(const Position& pos) {
    _pos = pos;
    updateModelMatrix();
}

void IncorporateRectangle3D::setSize(const Size3D& size) {
    _size = size;
    updateModelMatrix();
}

void IncorporateRectangle3D::setImage(const AbstractImage& image) {
    _renderData.setTexture("texture1", image.getTexture(ImageWrapMode::ClampToEdge));
    _textureEnable = true;
}

void IncorporateRectangle3D::setColor(const Color& color) {
    _color = color;
}

void IncorporateRectangle3D::updateUniformes() {
    _renderData.setUniform("imageEnable", _textureEnable);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _renderData.setUniform("modelMatrix", _modelMatrix);
}

Attitude3DController& IncorporateRectangle3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void IncorporateRectangle3D::mergeCopies(std::vector<IncorporateRectangle3D>& rectangles) {
    size_t instCount = rectangles.size();
    std::vector<Matrix4X4> modelMatrices(instCount);
    for (int i = 0; i < instCount; ++i) {
        IncorporateRectangle3D& rect = rectangles[i];
        rect.updateModelMatrix();
        modelMatrices[i] = rect._modelMatrix;
    }

    // OpenGL 不直接支持矩阵类型 mat4, 需要把它分成4个独立的 vec4 处理, 每个 vec4 必须单独绑定到一个顶点属性位置
    std::vector<ShaderAttribDescriptor> descs = {
        DESC("aInstanceMat-0", 3, Matrix4X4, _data0),
        DESC("aInstanceMat-1", 4, Matrix4X4, _data1),
        DESC("aInstanceMat-2", 5, Matrix4X4, _data2),
        DESC("aInstanceMat-3", 6, Matrix4X4, _data3),
    };
    _renderData.setInstanceVertices(modelMatrices, descs);
}

void IncorporateRectangle3D::updateRenderData() {
    std::vector<Instance3DVertex> vertices = {
        {{-0.5f, -0.5f, 0.0f},  {0.0f, 0.0f}},
        {{0.5f,  -0.5f, 0.0f},  {1.0f, 0.0f}},
        {{0.5f,  0.5f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f, 0.5f,  0.0f},  {0.0f, 1.0f}},
    };
    _renderData.setVertices(vertices);

    _renderData.setIndices({
        0, 1, 2,
        0, 2, 3
    });
}

void IncorporateRectangle3D::updateModelMatrix() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));

    glm::mat4 attitudeMatrix = _attitudeCtrl.getAttitudeMatrix();
    model = model * attitudeMatrix;

    model = glm::scale(model, glm::vec3(_size.x,  _size.y, 1.0f));

    memcpy(&_modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
}
