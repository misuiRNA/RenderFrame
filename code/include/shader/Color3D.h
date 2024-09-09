#ifndef _HEAD_FLAG_SHADER_COLOR3D_H
#define _HEAD_FLAG_SHADER_COLOR3D_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"

struct Color3D : AbstractShader {
    struct Vertex {
        Vertex(const Position& pos) : pos(pos) { }
        Position pos;
    };

    Color3D(bool isParallel);

    void setColor(const Color& color);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});
    void setVertexData(const RenderShape& shape);

private:
    void updateUniformes() override;

private:
    Position _pos;
    Size3D _size;
    Color _color;
};

#endif    // _HEAD_FLAG_SHADER_COLOR3D_H