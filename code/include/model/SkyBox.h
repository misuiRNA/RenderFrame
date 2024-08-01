#ifndef _HEAD_FLAG_MODEL_SKYBOX_H
#define _HEAD_FLAG_MODEL_SKYBOX_H

#include "model/AbstractDrawObject.h"
#include "BaseDefine.h"
#include "Image.h"

struct SkyBox : public AbstractDrawObject {
    struct Vertex {
        Position pos;
    };

    SkyBox();
    void setImage(const AbstractImage& image);
    void setCenter(const Position& center);
    void setVertexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});
private:
    void updateUniformes() override;
private:
    Attitude3DController _attitudeCtrl;
};


#endif // _HEAD_FLAG_MODEL_SKYBOX_H