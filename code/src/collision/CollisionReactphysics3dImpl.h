#ifndef _HEAD_FLAG_COLLISIONREACTPHYSICS3DIMPL_H
#define _HEAD_FLAG_COLLISIONREACTPHYSICS3DIMPL_H

#include "collision/Collision.h"
#include "reactphysics3d/reactphysics3d.h"


struct CollisionBodyReactphysics3dImpl : CollisionBody {
    CollisionBodyReactphysics3dImpl(const std::shared_ptr<rp3d::RigidBody>& handle);
    void setSpeed(float speed) override;
    Transform getTransform() const override;

private:
    std::shared_ptr<rp3d::RigidBody> _handle;
};


struct CollisionSystemReactphysics3dImpl : CollisionSystem {
    CollisionSystemReactphysics3dImpl();
    std::shared_ptr<CollisionBody> createCollisionBody(const Transform& trans) override;
    void update(double timeStep) override;

private:
    rp3d::PhysicsCommon physicsCommon;
    std::shared_ptr<rp3d::PhysicsWorld> _world;
    std::shared_ptr<rp3d::RigidBody> _ground;
};
#endif