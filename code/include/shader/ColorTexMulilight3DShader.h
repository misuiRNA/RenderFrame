#ifndef _HEAD_FLAG_MODEL_CUBOID_H
#define _HEAD_FLAG_MODEL_CUBOID_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"
#include "assets/Image.h"
#include "base/Transform.h"

struct ColorTexMulilight3DVertex {
    Position pos;
    Vector2D texCoord;
    Vector3D normal;
};

struct ColorTexMulilight3DShader : AbstractShader {
     using Vertex = ColorTexMulilight3DVertex;

    ColorTexMulilight3DShader(const Size3D& size);
    ColorTexMulilight3DShader(const ColorTexMulilight3DShader& oth);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setColor(const Color& color);
    void setPrimaryImage(const AbstractImage& image);
    void setSecondaryImage(const AbstractImage& image);
    void setMaterial(const ShaderMaterial& material);
    Transform& getTransform();

private:
    void updateUniformes() override;

private:
    Transform _trans;
    Color _color;

    bool _imageEnable;

public:
    static const ShaderAttribDescriptor DESCRIPTOR;
    static ColorTexMulilight3DVertex BuildVertex(const RenderShape::Vertex& data) {
        ColorTexMulilight3DVertex vertex;
        vertex.pos = data.pos;
        vertex.texCoord = data.texcoord;
        vertex.normal = data.normal;
        return vertex;
    }
};

#endif