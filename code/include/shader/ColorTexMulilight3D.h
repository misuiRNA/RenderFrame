#ifndef _HEAD_FLAG_MODEL_CUBOID_H
#define _HEAD_FLAG_MODEL_CUBOID_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"
#include "Image.h"
#include "Attitude3DController.h"

struct ColorTexMulilight3DVertex {
    ColorTexMulilight3DVertex(const RenderShape::Vertex& data) : pos(data.pos), texCoord(data.texcoord), normal(data.normal) { }
    Position pos;
    Vector2D texCoord;
    Vector3D normal;

    static const ShaderAttribDescriptor DESCRIPTOR;
};

struct ColorTexMulilight3D : AbstractShader<ColorTexMulilight3DVertex> {
    ColorTexMulilight3D(const Size3D& size);
    ColorTexMulilight3D(const ColorTexMulilight3D& oth);

    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setColor(const Color& color);
    void setPrimaryImage(const AbstractImage& image);
    void setSecondaryImage(const AbstractImage& image);
    void setMaterial(const ShaderMaterial& material);
    Attitude3DController& getAttituedeCtrl();

private:
    void updateUniformes() override;

private:
    Attitude3DController _attitudeCtrl;
    Color _color;

    bool _imageEnable;
};

#endif