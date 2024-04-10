#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderData.h"

struct AbstractModel {
    AbstractModel(const RenderData& renderData);
    void show();

private:
    virtual void updateRenderData() = 0;
    virtual void updateUniformes() = 0;

protected:
    RenderData _renderData;
    bool _needUpdateRenderData;

};

#endif
