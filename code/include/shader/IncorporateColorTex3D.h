#ifndef _HEAD_FLAG_MODEL_INCORPORATERECTANGLE3D_H
#define _HEAD_FLAG_MODEL_INCORPORATERECTANGLE3D_H

#include "shader/AbstractShader.h"
#include "BaseDefine.h"
#include "Image.h"
#include "Attitude3DController.h"

struct IncorporateColorTex3DVertex {
    IncorporateColorTex3DVertex(const RenderShape::Vertex& data) : pos(data.pos), texcoord(data.texcoord) { }
    Position pos;
    Vector2D texcoord;

    static const ShaderAttribDescriptor DESCRIPTOR;
};

struct IncorporateColorTex3D : AbstractShader<IncorporateColorTex3DVertex> {

    IncorporateColorTex3D(const Size3D& size);

    // TODO: 优化, 考虑优化设计提升opengl实例化属性的可复用性
    void mergeCopies(std::vector<IncorporateColorTex3D>& rectangles);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);
    Attitude3DController& getAttituedeCtrl();

private:
    void updateUniformes() override;

private:
    Attitude3DController _attitudeCtrl;
    Color _color;

    bool _textureEnable;
};

#endif
