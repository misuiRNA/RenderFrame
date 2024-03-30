#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "ShaderProgram.h"

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

#endif
