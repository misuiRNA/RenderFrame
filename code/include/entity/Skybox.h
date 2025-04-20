#ifndef _HEAD_FLAG_OBJECT_SKYBOX_H
#define _HEAD_FLAG_OBJECT_SKYBOX_H

#include "shader/ColorTexcubeShader.h"

struct Skybox {
    Skybox();
    void setImage(const AbstractImage& image);
    void show(const Position& center);
private:
    ColorTexcubeShader _shader;
};

#endif