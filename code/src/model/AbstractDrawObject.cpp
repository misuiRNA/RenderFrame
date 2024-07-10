#include "model/AbstractDrawObject.h"


AbstractDrawObject::AbstractDrawObject(ShaderProgram& prog, RenderDataMode mode)
: _renderData(prog, mode)
, _needUpdateRenderData(true) {

}

void AbstractDrawObject::show() {
    if (_needUpdateRenderData) {
        _needUpdateRenderData = false;
        updateRenderData();
    }
    updateUniformes();

    doDraw();
}

void AbstractDrawObject::doDraw() {
    _renderData.draw();
}
