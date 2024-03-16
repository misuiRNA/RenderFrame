#include "model/AbstractModel.h"

AbstractModel::AbstractModel(ShaderProgram& prog)
: _prog(prog)
, _renderData(_prog.getRenderData())
, _needUpdateRenderData(true) {

}

void AbstractModel::show() {
    if (_needUpdateRenderData) {
        _needUpdateRenderData = false;
        updateRenderData();
    }
    updateUniformes();
    _prog.draw(_renderData);
}
