#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "Shader.h"

struct AbstractModel {
    AbstractModel(ShaderProgram& _prog);
    void show();

private:
    void updateTextures();
    virtual void updateRenderData() = 0;
    virtual void updateUniformes() = 0;

protected:
    ShaderProgram& _prog;
    RenderData _renderData;
    bool _needUpdateRenderData;

};

struct Color {
    Color() : Color(0.0f, 0.0f, 0.0f) { }
    Color(float r, float g, float b) : r(r), g(g), b(b) { }

    float r;
    float g;
    float b;
};

struct Position {
    Position(float x, float y, float z) : x(x), y(y), z(z) { }

    float x;
    float y;
    float z;
};

#endif
