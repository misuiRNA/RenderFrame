#include "model/AbstractDrawObject.h"


AbstractDrawObject::AbstractDrawObject(ShaderProgram& prog)
: _renderData(prog)
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
