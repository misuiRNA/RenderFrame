#ifndef _HEAD_FLAG_MODEL_RECTANGLE2D_H
#define _HEAD_FLAG_MODEL_RECTANGLE2D_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"
#include "Image.h"

struct ColorTex2D : AbstractShader {
    struct Vertex {
        Vertex(Vector3D pos, Vector2D texCoord) : pos(pos), texCoord(texCoord){ }
        Vector3D pos;
        Vector2D texCoord;
    };

    ColorTex2D(float width, float height);

    void setPosition(const Position& pos);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void setVertexData(const RenderShape& shape);

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
