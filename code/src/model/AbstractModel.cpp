#include "model/AbstractModel.h"


AbstractModel::AbstractModel(const RenderData& renderData)
: _renderData(renderData)
, _needUpdateRenderData(true) {

}

void AbstractModel::show() {
    if (_needUpdateRenderData) {
        _needUpdateRenderData = false;
        updateRenderData();
    }
    updateUniformes();

    doDraw();
}

void AbstractModel::doDraw() {
    _renderData.draw();
}
