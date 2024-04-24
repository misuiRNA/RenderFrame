#include "model/AbstractModel.h"


AbstractModel::AbstractModel(ShaderProgram& prog)
: _renderData(prog)
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
