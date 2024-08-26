#ifndef _HEAD_FLAG_MODEL_SKYBOX_H
#define _HEAD_FLAG_MODEL_SKYBOX_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"
#include "Image.h"

struct ColorTexcube : public AbstractShader {
    struct Vertex {
        Position pos;
    };

    ColorTexcube();
    void setImage(const AbstractImage& image);
    void setCenter(const Position& center);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});
private:
    void updateUniformes() override;
private:
    Attitude3DController _attitudeCtrl;
};


#endif // _HEAD_FLAG_MODEL_SKYBOX_H