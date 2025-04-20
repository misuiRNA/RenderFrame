#ifndef _HEAD_FLAG_MODEL_RECTANGLE2D_H
#define _HEAD_FLAG_MODEL_RECTANGLE2D_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"
#include "assets/Image.h"

struct ColorTex2DVertex {
    ColorTex2DVertex(const RenderShape::Vertex& data) : pos(data.pos), texCoord(data.texcoord) { }
    Vector3D pos;
    Vector2D texCoord;

    static const ShaderAttribDescriptor DESCRIPTOR;
};

struct ColorTex2DShader : AbstractShader<ColorTex2DVertex> {
    ColorTex2DShader(float width, float height);

    void setPosition(const Position& pos);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);

private:
    void updateUniformes() override;

private:
    Position _pos;
    float _width;
    float _height;
    Color _color;

    bool _textureEnable;
};

#endif
