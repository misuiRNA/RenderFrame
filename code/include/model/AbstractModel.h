#ifndef _HEAD_FLAG_MODEL_ABSTRACTMODEL_H
#define _HEAD_FLAG_MODEL_ABSTRACTMODEL_H

#include "RenderData.h"

// TODO: 优化, AbstractModel及其子类目前不支持移动构造, 拷贝过程中会出现renderData失效的问题
struct AbstractModel {
    AbstractModel(const RenderData& renderData);
    void show();

    virtual ~AbstractModel() { }

private:
    virtual void updateRenderData() = 0;
    virtual void updateUniformes() = 0;
    virtual void doDraw();

protected:
    RenderData _renderData;
    bool _needUpdateRenderData;

};

#endif
