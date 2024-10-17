#ifndef _HEAD_FLAG_MODEL_COLORTEXCUBE_H
#define _HEAD_FLAG_MODEL_COLORTEXCUBE_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"
#include "Image.h"

struct ColorTexcubeVertex {
    ColorTexcubeVertex(const Position& pos) : pos(pos) { }
    ColorTexcubeVertex(const RenderShape::Vertex& data) : pos(data.pos) { }
    Position pos;

    static const ShaderAttribDescriptor DESCRIPTOR;
};

struct ColorTexcube : public AbstractShader<ColorTexcubeVertex> {
    ColorTexcube();

    void setImage(const AbstractImage& image);
    void setPosition(const Position& pos);

private:
    void updateUniformes() override;

private:
    Attitude3DController _attitudeCtrl;
};


#endif // _HEAD_FLAG_MODEL_SKYBOX_H