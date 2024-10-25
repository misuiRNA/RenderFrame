#ifndef MODELD_H
#define MODELD_H

#include "shader/Model3DShader.h"


struct Model3DDrawObject {
    Model3DDrawObject(std::string const& path);
    void setPosition(const Position& pos);
    void setSize(const Size3D& size);
    void setAttituedeCtrl(const Vector3D& up, const Vector3D& front);
    void show();

private:
    void load(const std::string& modelPath);
private:
    std::vector<Model3DShader> _meshDrawes;
};

#endif