#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "Shader.h"

struct AbstractModel {
    AbstractModel(ShaderProgram& _prog);
    virtual void buildModel() { }
    void draw();

protected:
    ShaderProgram& _prog;
    RenderData _renderData;

};

#endif
