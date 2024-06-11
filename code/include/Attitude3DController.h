#ifndef _HEAD_FLAG_ATTITUDE3DCONTROLLER_H
#define _HEAD_FLAG_ATTITUDE3DCONTROLLER_H

#include "BaseDefine.h"
#include <glm/glm.hpp>

struct Attitude3DController {
    Attitude3DController(const Vector3D& up, const Vector3D& front);

    Attitude3DController& setFront(const Vector3D& front);
    Attitude3DController& setUp(const Vector3D& up);
    // TODO: 优化, 消除接口对glm的依赖
    glm::mat4 getAttitudeMatrix() const;

    const Vector3D& getFront() const;
    const Vector3D& getUp() const ;

private:
    Vector3D _up;
    Vector3D _front;
};

#endif
