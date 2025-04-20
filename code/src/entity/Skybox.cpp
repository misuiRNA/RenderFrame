#include "entity/Skybox.h"


extern std::vector<ColorTexcubeVertex> CUBE_SKYBOX_VERTICES;

Skybox::Skybox() {
    _shader.setVertexData(CUBE_SKYBOX_VERTICES, {});
}

void Skybox::setImage(const AbstractImage& image) {
    _shader.setImage(image);
}

// TODO: 优化, 天空盒子实现方式依赖外部设置中心点位置, 需要优化. 优化思路 设置跟随物如相机
void Skybox::show(const Position& center) {
    _shader.setPosition(center);
    _shader.show();
}


std::vector<ColorTexcubeVertex> CUBE_SKYBOX_VERTICES = {
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
