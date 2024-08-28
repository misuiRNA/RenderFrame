#ifndef _HEAD_FLAG_OBJECT_SKYBOX_H
#define _HEAD_FLAG_OBJECT_SKYBOX_H

#include "shader/ColorTexcube.h"

struct Skybox {
    Skybox();
    void setImage(const AbstractImage& image);
    void show(const Position& center);
private:
    ColorTexcube _shader;
};

#endif