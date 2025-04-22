#ifndef _HEAD_FLAG_MODEL_INCORPORATERECTANGLE3D_H
#define _HEAD_FLAG_MODEL_INCORPORATERECTANGLE3D_H

#include "render/AbstractShader.h"
#include "base/BaseDefine.h"
#include "assets/Image.h"
#include "base/Transform.h"

struct IncorporateColorTex3DVertex {
    Position pos;
    Vector2D texcoord;
};

struct IncorporateColorTex3DShader : AbstractShader {
     using Vertex = IncorporateColorTex3DVertex;

    IncorporateColorTex3DShader(const Size3D& size);
    // TODO: 优化, 考虑优化设计提升opengl实例化属性的可复用性
    void mergeCopies(std::vector<IncorporateColorTex3DShader>& rectangles);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setImage(const AbstractImage& image);
    void setColor(const Color& color);
    Transform& getTransform();

private:
    void updateUniformes() override;

private:
    Transform _trans;
    Color _color;

    bool _textureEnable;

public:
    static const ShaderAttribDescriptor DESCRIPTOR;
    static IncorporateColorTex3DVertex BuildVertex(const RenderShape::Vertex& data) {
        IncorporateColorTex3DVertex vertex;
        vertex.pos = data.pos;
        vertex.texcoord = data.texcoord;
        return vertex;
    }
};

#endif
