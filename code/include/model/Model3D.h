#ifndef MODEL_H
#define MODEL_H

#include "model/AbstractDrawObject.h"
#include "Attitude3DController.h"
#include <string>
#include <map>
#include <vector>

struct Model3D : AbstractDrawObject {
    Model3D(std::string const& path);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    Attitude3DController& getAttituedeCtrl();

private:
    void updateRenderData() override;
    void updateUniformes() override;
    void doDraw() override;
    void updateModelMatrix();

private:
    Position _pos;
    Size3D _size;
    Attitude3DController _attitudeCtrl;
    float _modelMatrix[16] = { 0.0f };

    std::string _modelPath;
    std::vector<RenderData> _meshRenderDatas;
};

#endif