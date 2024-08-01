#ifndef _HEAD_FLAG_MODEL_RECTANGLE2D_H
#define _HEAD_FLAG_MODEL_RECTANGLE2D_H

#include "model/AbstractDrawObject.h"
#include "BaseDefine.h"
#include "Image.h"

struct Rectangle2D : AbstractDrawObject {
    struct Vertex {
        Vector3D pos;
        Vector2D texCoord;
    };

    Rectangle2D(float width, float height);

    void setPosition(const Position& pos);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

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
