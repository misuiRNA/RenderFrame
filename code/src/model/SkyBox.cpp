#include "model/SkyBox.h"
#include "Utils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static ShaderProgram& GetShaderProg() {
    static const std::string MODEL_NAME = "SkyBox";
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/SkyBox.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/SkyBox.fs");
    static const std::map<std::string, int> ATTRIBUTE_NAME_MAP = {
        {"aPos"     , 0},
    };
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR, ATTRIBUTE_NAME_MAP);
    return prog;
}

struct SkyBoxVertex {
    SkyBoxVertex(float x, float y, float z) : pos(x, y, z) { }
    Position pos;

    static std::vector<ShaderAttribDescriptor> descriptor;
};

std::vector<ShaderAttribDescriptor> SkyBoxVertex::descriptor = {
    DESC("aPos", 0, SkyBoxVertex, pos),
};

SkyBox::SkyBox()
: AbstractDrawObject(GetShaderProg()) {

}

void SkyBox::setImage(const AbstractImage& image) {
    _renderData.setTexture("skybox", image.getTexture(ImageWrapMode::ClampToEdge));
}

// TODO: 优化, 天空盒子实现方式依赖外部设置中心点位置, 需要优化
void SkyBox::setCenter(const Position& center) {
    _center = center;
}

void SkyBox::updateUniformes() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(_center.x, _center.y, _center.z));
    // 调整天空盒子姿态
    Attitude3DController attitudeCtrl({0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f});
    glm::mat4 attitudeMatrix = attitudeCtrl.getAttitudeMatrix();
    model = model * attitudeMatrix;
    _renderData.setUniformMat4("modelMatrix", glm::value_ptr(model));
}

void SkyBox::updateRenderData() {
    std::vector<SkyBoxVertex> vertices = {
        {-1.0f,  1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},

        {-1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},

        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f,  1.0f},
        {1.0f,  1.0f,  1.0f},
        {1.0f,  1.0f,  1.0f},
        {1.0f,  1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},

        {-1.0f, -1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        {1.0f,  1.0f,  1.0f},
        {1.0f,  1.0f,  1.0f},
        {1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},

        {-1.0f,  1.0f, -1.0f},
        {1.0f,  1.0f, -1.0f},
        {1.0f,  1.0f,  1.0f},
        {1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},

        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f},
        {1.0f, -1.0f,  1.0f}
    };
    _renderData.setVertices(vertices);
}
