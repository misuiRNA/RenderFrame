#ifndef MODEL3D_H
#define MODEL3D_H

#include "render/AbstractShader.h"
#include "base/Transform.h"
#include "assets/Image.h"
#include <string>
#include <map>
#include <vector>

struct Model3DVertex {
    Model3DVertex(Position pos, Vector3D normal, Vector2D texCoord) : pos(pos), texCoords(texCoord), normal(normal) { }
    Vector3D pos;
    Vector3D normal;
    Vector2D texCoords;
};

struct Model3DShader : AbstractShader {
     using Vertex = Model3DVertex;

    Model3DShader();
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setDiffuseImage(int index, const AbstractImage& image);
    void setSpecularImage(int index, const AbstractImage& image);
    void setNormalImage(int index, const AbstractImage& image);
    void setHeightImage(int index, const AbstractImage& image);
    Transform& getTransform();

private:
    void updateUniformes() override;

private:
    Transform _trans;

public:
    static const ShaderAttribDescriptor DESCRIPTOR;
    static Model3DVertex BuildVertex(const RenderShape::Vertex& data) {
        Model3DVertex vertex(data.pos, data.normal, data.texcoord);
        // vertex.pos = data.pos;
        // vertex.normal = data.normal;
        // vertex.texCoords = data.texcoord;
        return vertex;
    }
};

#endif