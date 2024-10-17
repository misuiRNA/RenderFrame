#ifndef MODEL3D_H
#define MODEL3D_H

#include "shader/AbstractShader.h"
#include "Attitude3DController.h"
#include "Image.h"
#include <string>
#include <map>
#include <vector>

struct Model3DVertex {
    Model3DVertex(Position pos, Vector3D normal, Vector2D texCoord) : pos(pos), texCoords(texCoord), normal(normal) { }
    Model3DVertex(const RenderShape::Vertex& data) : pos(data.pos), normal(data.normal), texCoords(data.texcoord) { }
    Vector3D pos;
    Vector3D normal;
    Vector2D texCoords;

    static const ShaderAttribDescriptor DESCRIPTOR;
};

struct Model3D : AbstractShader<Model3DVertex> {

    Model3D();
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setDiffuseImage(int index, const AbstractImage& image);
    void setSpecularImage(int index, const AbstractImage& image);
    void setNormalImage(int index, const AbstractImage& image);
    void setHeightImage(int index, const AbstractImage& image);
    Attitude3DController& getAttituedeCtrl();

private:
    void updateUniformes() override;

private:
    Attitude3DController _attitudeCtrl;
};

#endif