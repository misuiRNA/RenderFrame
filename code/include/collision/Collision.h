#ifndef _HEAD_FLAG_COLLISION_H
#define _HEAD_FLAG_COLLISION_H

#include <memory>
#include "BaseDefine.h"
#include "Attitude3DController.h"


struct CollisionBody {
    virtual void setSpeed(float speed) = 0;
    virtual Attitude3DController getAttituedeCtrl() const = 0;
};

struct CollisionSystem {
    static CollisionSystem& getInstance();

    virtual std::shared_ptr<CollisionBody> createCollisionBody(const Attitude3DController& atti) = 0;
    virtual void update(double timeStep) = 0;
};

#endif