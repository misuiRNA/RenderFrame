#include "shader/IncorporateColorTex3DShader.h"
#include "ShaderProgram.h"
#include "base/Utils.h"
#include "assets/UtilsFileSys.h"


const ShaderAttribDescriptor IncorporateColorTex3DShader::DESCRIPTOR = DESC_NEW(IncorporateColorTex3DVertex,
    ITEM(IncorporateColorTex3DVertex, 0, "aPos",      pos),
    ITEM(IncorporateColorTex3DVertex, 1, "aTexCoord", texcoord),
);

// TODO: 优化, 1.shader字符串编译时确定，不读取文件；2.返回的路径位置应为可执行文件位置，而不是执行命令的位置 考虑使用 std::filesystem
static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/Instanced3DShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/shader/glsl/Instanced3DShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


IncorporateColorTex3DShader::IncorporateColorTex3DShader(const Size3D& size)
: AbstractShader(GetShaderProg())
, _trans({0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {size.x, size.y, 1.0f})
, _textureEnable(false) {

}

void IncorporateColorTex3DShader::setPosition(const Position& pos) {
    _trans.setPosition(pos);
}

void IncorporateColorTex3DShader::setSize(const Size3D& size) {
    _trans.setSize({size.x, size.y, 1.0f});
}

void IncorporateColorTex3DShader::setImage(const AbstractImage& image) {
    setTexture("texture1", image.getTexture(ImageWrapMode::ClampToEdge));
    _textureEnable = true;
}

void IncorporateColorTex3DShader::setColor(const Color& color) {
    _color = color;
}

void IncorporateColorTex3DShader::updateUniformes() {
    setUniform("imageEnable", _textureEnable);
    setUniform("color", _color.r, _color.g, _color.b, 1.0f);
    setUniform("modelMatrix", _trans.getMatrix());
}

Transform& IncorporateColorTex3DShader::getTransform() {
    return _trans;
}

void IncorporateColorTex3DShader::mergeCopies(std::vector<IncorporateColorTex3DShader>& rectangles) {
    size_t instCount = rectangles.size();
    std::vector<Matrix4X4> modelMatrices(instCount);
    for (int i = 0; i < instCount; ++i) {
        modelMatrices[i] = rectangles[i]._trans.getMatrix();
    }

    // OpenGL 不直接支持矩阵类型 mat4, 需要把它分成4个独立的 vec4 处理, 每个 vec4 必须单独绑定到一个顶点属性位置
    static const ShaderAttribDescriptor desc = DESC_NEW(Matrix4X4,
        ITEM(Matrix4X4, 3, "aInstanceMat-0", _data0),
        ITEM(Matrix4X4, 4, "aInstanceMat-1", _data1),
        ITEM(Matrix4X4, 5, "aInstanceMat-2", _data2),
        ITEM(Matrix4X4, 6, "aInstanceMat-3", _data3),
    );
    setInstanceVertices(modelMatrices.size(), modelMatrices.data(), desc);
}
