#ifndef MODEL_H
#define MODEL_H

#include "model/AbstractModel.h"
#include <string>
#include <map>
#include <vector>

struct Model3D : AbstractModel {
    Model3D(std::string const& path);
    void setPosition(const Position& pos);
    void setFront(const Vector3D& front);
    void setScale(float scale);

private:
    void updateRenderData() override;
    void updateUniformes() override;

private:
    Position _pos;
    Vector3D _front;
    Vector3D _up;
    float _scaleRatio;

    std::string _modelPath;
};

#endif