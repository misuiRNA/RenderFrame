#include "shader/AbstractShader.h"


AbstractShader::AbstractShader(ShaderProgram& prog, RenderDataMode mode)
: _renderData(prog, mode) {

}

void AbstractShader::show() {
    updateUniformes();

    doDraw();
}

void AbstractShader::doDraw() {
    _renderData.draw();
}
