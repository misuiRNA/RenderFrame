#ifndef _HEAD_FLAG_SHADER_COLOR3D_H
#define _HEAD_FLAG_SHADER_COLOR3D_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"

struct Color3DVertex {
    Position pos;
};

struct Color3DShader : AbstractShader {
    using Vertex = Color3DVertex;

    Color3DShader(bool isParallel);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setColor(const Color& color);

private:
    void updateUniformes() override;

private:
    Position _pos;
    Size3D _size;
    Color _color;

public:

    static const ShaderAttribDescriptor DESCRIPTOR;
    static Color3DVertex BuildVertex(const RenderShape::Vertex& data) {
        Color3DVertex vertex;
        vertex.pos = data.pos;
        return vertex;
    }
};

#endif    // _HEAD_FLAG_SHADER_COLOR3D_H