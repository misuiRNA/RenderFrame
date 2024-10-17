#ifndef _HEAD_FLAG_SHADER_COLOR3D_H
#define _HEAD_FLAG_SHADER_COLOR3D_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"

struct Color3DVertex {
    Color3DVertex(const RenderShape::Vertex& data) : pos(data.pos) { }
    Position pos;

    static const ShaderAttribDescriptor DESCRIPTOR;
};

struct Color3D : AbstractShader<Color3DVertex> {
    Color3D(bool isParallel);

    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setColor(const Color& color);

private:
    void updateUniformes() override;

private:
    Position _pos;
    Size3D _size;
    Color _color;
};

#endif    // _HEAD_FLAG_SHADER_COLOR3D_H