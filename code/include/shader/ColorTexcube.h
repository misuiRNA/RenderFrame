#ifndef _HEAD_FLAG_MODEL_COLORTEXCUBE_H
#define _HEAD_FLAG_MODEL_COLORTEXCUBE_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"
#include "Image.h"

struct ColorTexcube : public AbstractShader {
    struct Vertex {
        Vertex(const Position& pos) : pos(pos) { }
        Position pos;
    };

    ColorTexcube();
    void setImage(const AbstractImage& image);
    void setPosition(const Position& pos);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});
    void setVertexData(const RenderShape& shape);
private:
    void updateUniformes() override;
private:
    Attitude3DController _attitudeCtrl;
};


#endif // _HEAD_FLAG_MODEL_SKYBOX_H