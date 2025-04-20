#ifndef _HEAD_FLAG_COLLISION_H
#define _HEAD_FLAG_COLLISION_H

#include <memory>
#include "base/BaseDefine.h"
#include "base/Transform.h"


struct CollisionBody {
    virtual void setSpeed(float speed) = 0;
    virtual Transform getTransform() const = 0;
};

struct CollisionSystem {
    static CollisionSystem& getInstance();

    virtual std::shared_ptr<CollisionBody> createCollisionBody(const Transform& trans) = 0;
    virtual void update(double timeStep) = 0;
};

#endif