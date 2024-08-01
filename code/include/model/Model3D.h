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
    void load(const std::string& modelPath);
    void updateUniformes() override;
    void doDraw() override;

private:
    Attitude3DController _attitudeCtrl;

    std::string _modelPath;
    std::vector<RenderData> _meshRenderDatas;
};

#endif