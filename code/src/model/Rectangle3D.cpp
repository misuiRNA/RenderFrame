#include "model/Rectangle3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Utils.h"

struct Rectangle3DVertex {
    Position pos;
    Vector2D texcoord;
};

// TODO: 优化, 1.shader字符串编译时确定，不读取文件；2.返回的路径位置应为可执行文件位置，而不是执行命令的位置 考虑使用 std::filesystem
static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Rectangle3DShader.fs");
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos",      0, Rectangle3DVertex, pos),
        DESC("aTexCoord", 1, Rectangle3DVertex, texcoord),
    };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, descriptor);
    return prog;
}


Rectangle3D::Rectangle3D(const Size3D& size)
: AbstractDrawObject(GetShaderProg(), RenderDataMode::TRIANGLES)
, _pos(0.0f, 0.0f, 0.0f)
, _size(size)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f})
, _textureEnable(false) {
    updateModelMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this](){ updateModelMatrix(); });
}

Rectangle3D::Rectangle3D(const Rectangle3D& oth)
: AbstractDrawObject(oth)
, _pos(oth._pos)
, _size(oth._size)
, _attitudeCtrl(oth._attitudeCtrl.getUp(), oth._attitudeCtrl.getFront())
, _textureEnable(oth._textureEnable) {
    updateModelMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this](){ updateModelMatrix(); });
}

void Rectangle3D::setPosition(const Position& pos) {
    _pos = pos;
    updateModelMatrix();
}

void Rectangle3D::setSize(const Size3D& size) {
    _size = size;
    updateModelMatrix();
}

void Rectangle3D::setImage(const AbstractImage& image) {
    _renderData.setTexture("texture1", image.getTexture(ImageWrapMode::ClampToEdge));
    _textureEnable = true;
}

void Rectangle3D::setColor(const Color& color) {
    _color = color;
}

void Rectangle3D::updateUniformes() {
    _renderData.setUniform("imageEnable", _textureEnable);
    _renderData.setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    _renderData.setUniformMat4("modelMatrix", _modelMatrix);
    _renderData.setUniform("useInstanceModel", _useInstancemodel);
}

Attitude3DController& Rectangle3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void Rectangle3D::expandAsGroup(std::vector<Rectangle3D>& rectangles) {
    struct ModelMatrices {
        float _data0[4] = { 0.0f };
        float _data1[4] = { 0.0f };
        float _data2[4] = { 0.0f };
        float _data3[4] = { 0.0f };
    };

    size_t instCount = rectangles.size();
    std::vector<ModelMatrices> modelMatrices(instCount);
    for (int i = 0; i < instCount; ++i) {
        Rectangle3D& rect = rectangles[i];
        rect.updateModelMatrix();
        memcpy(&modelMatrices[i], rect._modelMatrix, sizeof(ModelMatrices));
    }

    // OpenGL 的顶点属性指针只支持标量和向量类型（如 float、vec2、vec3、vec4），而不直接支持矩阵类型（如 mat4）。
    // 一个 mat4 矩阵在内存中是连续存储的，但是 OpenGL 需要将其分成4个独立的 vec4 向量来处理。
    // 因此，每个 vec4 向量必须单独绑定到一个顶点属性位置。
    std::vector<ShaderAttribDescriptor> descs = {
        DESC("aInstanceMat-0", 3, ModelMatrices, _data0),
        DESC("aInstanceMat-1", 4, ModelMatrices, _data1),
        DESC("aInstanceMat-2", 5, ModelMatrices, _data2),
        DESC("aInstanceMat-3", 6, ModelMatrices, _data3),
    };
    _renderData.setInstanceVertices(modelMatrices, descs);
    _useInstancemodel = true;
}

void Rectangle3D::updateRenderData() {
    std::vector<Rectangle3DVertex> vertices = {
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

void Rectangle3D::updateModelMatrix() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));

    glm::mat4 attitudeMatrix = _attitudeCtrl.getAttitudeMatrix();
    model = model * attitudeMatrix;

    model = glm::scale(model, glm::vec3(_size.x,  _size.y, 1.0f));

    memcpy(_modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
}
