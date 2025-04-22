#ifndef _HEAD_FLAG_MODEL_RECTANGLE_H
#define _HEAD_FLAG_MODEL_RECTANGLE_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"
#include "assets/Image.h"
#include "base/Transform.h"

struct ColorTex3DVertex {
    Position pos;
    Vector2D texcoord;
    Color color;
};

struct ColorTex3DShader : AbstractShader {
     using Vertex = ColorTex3DVertex;

    ColorTex3DShader(const Size3D& size);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);
    Transform& getTransform();

private:
    void updateUniformes() override;

private:
    Transform _trans;
    Color _color;

    bool _textureEnable;

public:
    static const ShaderAttribDescriptor DESCRIPTOR;
    static ColorTex3DVertex BuildVertex(const RenderShape::Vertex& data) {
        ColorTex3DVertex vertex;
        vertex.pos = data.pos;
        vertex.texcoord = data.texcoord;
        vertex.color = data.color;
        return vertex;
    }
};

#endif
