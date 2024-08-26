#ifndef _HEAD_FLAG_MODEL_RECTANGLE_H
#define _HEAD_FLAG_MODEL_RECTANGLE_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"
#include "Image.h"
#include "Attitude3DController.h"

struct ColorTex3D : AbstractShader {
    struct Vertex {
        Position pos;
        Vector2D texcoord;
    };

    ColorTex3D(const Size3D& size);

    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});
    Attitude3DController& getAttituedeCtrl();

private:
    void updateUniformes() override;

private:
    Attitude3DController _attitudeCtrl;
    Color _color;

    bool _textureEnable;
};

#endif
