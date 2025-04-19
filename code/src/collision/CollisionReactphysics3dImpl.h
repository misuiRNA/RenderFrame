#ifndef _HEAD_FLAG_COLLISIONREACTPHYSICS3DIMPL_H
#define _HEAD_FLAG_COLLISIONREACTPHYSICS3DIMPL_H

#include "collision/Collision.h"
#include "reactphysics3d/reactphysics3d.h"


using namespace reactphysics3d;


struct CollisionBodyReactphysics3dImpl : CollisionBody {
    CollisionBodyReactphysics3dImpl(const std::shared_ptr<RigidBody>& handle);
    void setSpeed(float speed) override;
    Attitude3DController getAttituedeCtrl() const override;

private:
    std::shared_ptr<RigidBody> _handle;
};


struct CollisionSystemReactphysics3dImpl : CollisionSystem {
    CollisionSystemReactphysics3dImpl();
    std::shared_ptr<CollisionBody> createCollisionBody(const Attitude3DController& atti) override;
    void update(double timeStep) override;

private:
    PhysicsCommon physicsCommon;
    std::shared_ptr<PhysicsWorld> _world;
    std::shared_ptr<RigidBody> _ground;
};
#endif