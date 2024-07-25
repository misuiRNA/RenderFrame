#include "model/SkyBox.h"
#include "Utils.h"


struct SkyBoxVertex {
    Position pos;
};

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/SkyBox.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/SkyBox.fs");
    static const std::vector<ShaderAttribDescriptor> descriptor = {
        DESC("aPos", 0, SkyBoxVertex, pos),
    };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, descriptor);
    return prog;
}


SkyBox::SkyBox()
: AbstractDrawObject(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}) {

}

void SkyBox::setImage(const AbstractImage& image) {
    _renderData.setTexture("skybox", image.getTexture(ImageWrapMode::ClampToEdge));
}

// TODO: 优化, 天空盒子实现方式依赖外部设置中心点位置, 需要优化
void SkyBox::setCenter(const Position& center) {
    _attitudeCtrl.setPosition(center);
}

void SkyBox::updateUniformes() {
    _renderData.setUniform("modelMatrix", _attitudeCtrl.getMatrix());
}

void SkyBox::updateRenderData() {
    std::vector<SkyBoxVertex> vertices = {
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
    _renderData.setVertices(vertices);
}
