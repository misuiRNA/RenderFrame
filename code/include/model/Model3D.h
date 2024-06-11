#ifndef MODEL_H
#define MODEL_H

#include "model/AbstractModel.h"
#include "Attitude3DController.h"
#include <string>
#include <map>
#include <vector>

struct Model3D : AbstractModel {
    Model3D(std::string const& path);
    void setPosition(const Position& pos);
    void setScale(float scale);
    Attitude3DController& getAttituedeCtrl();

private:
    void updateRenderData() override;
    void updateUniformes() override;
    void doDraw() override;

private:
    Position _pos;
    Attitude3DController _attitudeCtrl;
    float _scaleRatio;

    std::string _modelPath;
    std::vector<RenderData> _meshRenderDatas;
};


struct Model3DVertex {
    Model3DVertex(Vector3D pos, Vector3D normal, Vector2D texCoords);

    Vector3D pos;
    Vector3D normal;
    Vector2D texCoords;

    static std::vector<ShaderAttribDescriptor> descriptor;
};

#endif