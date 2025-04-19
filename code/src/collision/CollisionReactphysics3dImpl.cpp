#include "CollisionReactphysics3dImpl.h"


CollisionSystem& CollisionSystem::getInstance() {
    static CollisionSystemReactphysics3dImpl inst;
    return inst;
}


CollisionBodyReactphysics3dImpl::CollisionBodyReactphysics3dImpl(const std::shared_ptr<RigidBody>& handle)
: _handle(handle) {

}

void CollisionBodyReactphysics3dImpl::setSpeed(float speed) {
    const Quaternion& quat =_handle->getTransform().getOrientation();
    _handle->setLinearVelocity(Vector3(quat.x * speed, quat.y * speed, quat.z * speed));
}

Attitude3DController CollisionBodyReactphysics3dImpl::getAttituedeCtrl() const {
    Attitude3DController ret({1.0, 0.0, 0.0}, {0.0, 0.0, 1.0});
    if (_handle) {
        Transform trans = _handle->getTransform();
        const Vector3& pos = trans.getPosition();
        const Quaternion& ot = trans.getOrientation();
        ret.setPosition({pos.x, pos.y, pos.z});
        ret.setFront({ot.x, ot.y, ot.z});
    }
    return ret;
}


CollisionSystemReactphysics3dImpl::CollisionSystemReactphysics3dImpl() {
    std::shared_ptr<PhysicsWorld> world(physicsCommon.createPhysicsWorld(), [this](PhysicsWorld* w) { physicsCommon.destroyPhysicsWorld(w); });
    world->setGravity(Vector3(0.0, 0.0, -9.81f));
    // 创建地面
    Transform groundTransform(Vector3(0.0, 0.0, -1.0), Quaternion::identity());
    std::shared_ptr<RigidBody> ground(world->createRigidBody(groundTransform), [this](RigidBody* b) { if (_world) _world->destroyRigidBody(b); });
    BoxShape* groundShape = physicsCommon.createBoxShape(Vector3(100.0, 100.0, 1.0));    // 不需要手动释放, 由 physicsCommon 管理
    ground->addCollider(groundShape, Transform::identity());
    ground->setType(BodyType::STATIC);

    _world = world;
    _ground = ground;
}

std::shared_ptr<CollisionBody> CollisionSystemReactphysics3dImpl::createCollisionBody(const Attitude3DController& atti) {
    const Position& pos = atti.getPosition();
    const Vector3D& front = atti.getFront();
    Size3D size = 0.5 * atti.getSize();

    // TODO: 尽量复用 boxShape, 减少性能开销
    BoxShape* boxShape = physicsCommon.createBoxShape(Vector3(size.x, size.y, size.z));
    Transform boxTransform(Vector3(pos.x, pos.y, pos.z), Quaternion(front.x, front.y, front.z, 1.0f));
    std::shared_ptr<RigidBody> box(_world->createRigidBody(boxTransform), [this](RigidBody* rb) { if (_world) _world->destroyRigidBody(rb); });
    box->addCollider(boxShape, Transform::identity());
    box->setMass(1.0f);

    std::shared_ptr<CollisionBody> collision = std::make_shared<CollisionBodyReactphysics3dImpl>(box);
    return collision;
}

void CollisionSystemReactphysics3dImpl::update(double timeStep) {
    if (_world) {
        _world->update(timeStep);
    }
}
