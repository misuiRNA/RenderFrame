#ifndef _HEAD_FLAG_MODEL_CUBOID_H
#define _HEAD_FLAG_MODEL_CUBOID_H

#include "model/AbstractDrawObject.h"
#include "BaseDefine.h"
#include "Image.h"
#include "Attitude3DController.h"

struct Cuboid : AbstractDrawObject {
    struct Vertex {
        Position pos;
        Vector2D texCoord;
        Vector3D normal;
    };

    Cuboid(const Size3D& size);
    Cuboid(const Cuboid& oth);

    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setColor(const Color& color);
    void addImage(const AbstractImage& image);
    void setMaterial(const ShaderMaterial& material);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});
    
    Attitude3DController& getAttituedeCtrl();

private:
    void updateUniformes() override;

private:
    Attitude3DController _attitudeCtrl;
    Color _color;

    int _imageCount;
};

#endif