#ifndef _HEAD_FLAG_MODEL_COLORTEXCUBE_H
#define _HEAD_FLAG_MODEL_COLORTEXCUBE_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"
#include "assets/Image.h"

struct ColorTexcubeVertex {
    ColorTexcubeVertex(const Position& pos) : pos(pos) { }
    Position pos;
};

struct ColorTexcubeShader : public AbstractShader {
     using Vertex = ColorTexcubeVertex;

    ColorTexcubeShader();
    void setImage(const AbstractImage& image);
    void setPosition(const Position& pos);

private:
    void updateUniformes() override;

private:
    Transform _trans;

public:
    static const ShaderAttribDescriptor DESCRIPTOR;
    static ColorTexcubeVertex BuildVertex(const RenderShape::Vertex& data) {
        ColorTexcubeVertex vertex(data.pos);
        // vertex.pos = data.pos;
        return vertex;
    }
};


#endif // _HEAD_FLAG_MODEL_SKYBOX_H