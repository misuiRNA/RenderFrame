#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderData.h"

struct AbstractShader {
    AbstractShader(ShaderProgram& prog, RenderDataMode mode);
    void show();

    virtual ~AbstractShader() { }

private:
    virtual void updateUniformes() = 0;
    virtual void doDraw();

protected:
    RenderData _renderData;
};

#endif
