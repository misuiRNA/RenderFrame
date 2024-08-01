#include "model/AbstractDrawObject.h"


AbstractDrawObject::AbstractDrawObject(ShaderProgram& prog, RenderDataMode mode)
: _renderData(prog, mode) {

}

void AbstractDrawObject::show() {
    updateUniformes();

    doDraw();
}

void AbstractDrawObject::doDraw() {
    _renderData.draw();
}
