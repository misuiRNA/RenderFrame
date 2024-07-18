#ifndef _HEAD_FLAG_MODEL_SKYBOX_H
#define _HEAD_FLAG_MODEL_SKYBOX_H

#include "model/AbstractDrawObject.h"
#include "BaseDefine.h"
#include "Image.h"

class SkyBox : public AbstractDrawObject {
public:
    SkyBox();
    void setImage(const AbstractImage& image);
    void setCenter(const Position& center);
private:
    void updateRenderData() override;
    void updateUniformes() override;
private:
    Position _center;
};


#endif // _HEAD_FLAG_MODEL_SKYBOX_H