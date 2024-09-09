#ifndef MODEL3D_H
#define MODEL3D_H

#include "shader/AbstractShader.h"
#include "Attitude3DController.h"
#include <string>
#include <map>
#include <vector>

struct Model3D : AbstractShader {
    struct Vertex {
        Vector3D pos;
        Vector3D normal;
        Vector2D texCoords;
    };

    Model3D();
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setTexture(const std::string& name, unsigned int textureId);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    Attitude3DController& getAttituedeCtrl();

private:
    void updateUniformes() override;

private:
    Attitude3DController _attitudeCtrl;
};

#endif