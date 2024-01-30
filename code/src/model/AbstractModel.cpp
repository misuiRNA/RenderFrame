#include "model/AbstractModel.h"

AbstractModel::AbstractModel(ShaderProgram& prog)
: _prog(prog)
, _renderData(_prog.buildRenderData()) {
    buildModel();
}

void AbstractModel::draw() {
    _prog.draw(_renderData);
}
