#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderData.h"

struct AbstractDrawObject {
    AbstractDrawObject(ShaderProgram& prog, RenderDataMode mode);
    void show();

    virtual ~AbstractDrawObject() { }

private:
    virtual void updateRenderData() = 0;
    virtual void updateUniformes() = 0;
    virtual void doDraw();

protected:
    RenderData _renderData;
    bool _needUpdateRenderData;

};

#endif
