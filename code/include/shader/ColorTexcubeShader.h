#ifndef _HEAD_FLAG_MODEL_COLORTEXCUBE_H
#define _HEAD_FLAG_MODEL_COLORTEXCUBE_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"
#include "assets/Image.h"

struct ColorTexcubeVertex {
    ColorTexcubeVertex(const Position& pos) : pos(pos) { }
    ColorTexcubeVertex(const RenderShape::Vertex& data) : pos(data.pos) { }
    Position pos;

    static const ShaderAttribDescriptor DESCRIPTOR;
};

struct ColorTexcubeShader : public AbstractShader<ColorTexcubeVertex> {
    ColorTexcubeShader();

    void setImage(const AbstractImage& image);
    void setPosition(const Position& pos);

private:
    void updateUniformes() override;

private:
    Transform _trans;
};


#endif // _HEAD_FLAG_MODEL_SKYBOX_H