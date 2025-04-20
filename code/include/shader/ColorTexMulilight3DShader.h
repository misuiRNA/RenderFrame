#ifndef _HEAD_FLAG_MODEL_CUBOID_H
#define _HEAD_FLAG_MODEL_CUBOID_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"
#include "assets/Image.h"
#include "base/Transform.h"

struct ColorTexMulilight3DVertex {
    ColorTexMulilight3DVertex(const RenderShape::Vertex& data) : pos(data.pos), texCoord(data.texcoord), normal(data.normal) { }
    Position pos;
    Vector2D texCoord;
    Vector3D normal;

    static const ShaderAttribDescriptor DESCRIPTOR;
};

struct ColorTexMulilight3DShader : AbstractShader<ColorTexMulilight3DVertex> {
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
};

#endif