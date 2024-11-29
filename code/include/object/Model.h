#ifndef MODELD_H
#define MODELD_H

#include "shader/Model3DShader.h"


struct Box3D {
    Box3D() : top(0), bottom(0), right(0), left(0), front(0), back(0) {}
    float top;    // +Y
    float bottom; // -Y
    float right;  // +X
    float left;   // -X
    float front;  // +Z
    float back;   // -Z
    float lenX() const { return right - left; }
    float lenY() const { return top - bottom; }
    float lenZ() const { return front - back; }
};

struct Model3DDrawObject {
    Model3DDrawObject(std::string const& path);
    void setPosition(const Position& pos);
    void move(const Position& offset);
    void setSize(const Size3D& size);
    void setAttituedeCtrl(const Vector3D& up, const Vector3D& front);
    void show();
    Position getPosition();

private:
    void load(const std::string& modelPath);
private:
    Position _pos;
    std::vector<Model3DShader> _meshDrawes;
    Box3D _modelBox;
};

#endif