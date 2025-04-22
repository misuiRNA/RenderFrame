#ifndef _HEAD_FLAG_MODEL_RECTANGLE2D_H
#define _HEAD_FLAG_MODEL_RECTANGLE2D_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"
#include "assets/Image.h"

struct ColorTex2DVertex {
    Vector3D pos;
    Vector2D texCoord;
};

struct ColorTex2DShader : AbstractShader {
     using Vertex = ColorTex2DVertex;

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

public:
    static const ShaderAttribDescriptor DESCRIPTOR;
    static ColorTex2DVertex BuildVertex(const RenderShape::Vertex& data) {
        ColorTex2DVertex vertex;
        vertex.pos = data.pos;
        vertex.texCoord = data.texcoord;
        return vertex;
    }
};

#endif
