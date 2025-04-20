#include "CollisionReactphysics3dImpl.h"


CollisionSystem& CollisionSystem::getInstance() {
    static CollisionSystemReactphysics3dImpl inst;
    return inst;
}


CollisionBodyReactphysics3dImpl::CollisionBodyReactphysics3dImpl(const std::shared_ptr<rp3d::RigidBody>& handle)
: _handle(handle) {

}

void CollisionBodyReactphysics3dImpl::setSpeed(float speed) {
    const rp3d::Quaternion& quat =_handle->getTransform().getOrientation();
    _handle->setLinearVelocity(rp3d::Vector3(quat.x * speed, quat.y * speed, quat.z * speed));
}

Transform CollisionBodyReactphysics3dImpl::getTransform() const {
    Transform ret({1.0, 0.0, 0.0}, {0.0, 0.0, 1.0});
    if (_handle) {
        rp3d::Transform trans = _handle->getTransform();
        const rp3d::Vector3& pos = trans.getPosition();
        const rp3d::Quaternion& ot = trans.getOrientation();
        ret.setPosition({pos.x, pos.y, pos.z});
        ret.setFront({ot.x, ot.y, ot.z});
    }
    return ret;
}


CollisionSystemReactphysics3dImpl::CollisionSystemReactphysics3dImpl() {
    std::shared_ptr<rp3d::PhysicsWorld> world(physicsCommon.createPhysicsWorld(), [this](rp3d::PhysicsWorld* w) { physicsCommon.destroyPhysicsWorld(w); });
    world->setGravity(rp3d::Vector3(0.0, 0.0, -9.81f));
    // 创建地面
    rp3d::Transform groundTransform(rp3d::Vector3(0.0, 0.0, -1.0), rp3d::Quaternion::identity());
    std::shared_ptr<rp3d::RigidBody> ground(world->createRigidBody(groundTransform), [this](rp3d::RigidBody* b) { if (_world) _world->destroyRigidBody(b); });
    rp3d::BoxShape* groundShape = physicsCommon.createBoxShape(rp3d::Vector3(100.0, 100.0, 1.0));    // 不需要手动释放, 由 physicsCommon 管理
    ground->addCollider(groundShape, rp3d::Transform::identity());
    ground->setType(rp3d::BodyType::STATIC);

    _world = world;
    _ground = ground;
}

std::shared_ptr<CollisionBody> CollisionSystemReactphysics3dImpl::createCollisionBody(const Transform& trans) {
    const Position& pos = trans.getPosition();
    const Vector3D& front = trans.getFront();
    Size3D size = 0.5 * trans.getSize();

    // TODO: 尽量复用 boxShape, 减少性能开销
    rp3d::BoxShape* boxShape = physicsCommon.createBoxShape(rp3d::Vector3(size.x, size.y, size.z));
    rp3d::Transform boxTransform(rp3d::Vector3(pos.x, pos.y, pos.z), rp3d::Quaternion(front.x, front.y, front.z, 1.0f));
    std::shared_ptr<rp3d::RigidBody> box(_world->createRigidBody(boxTransform), [this](rp3d::RigidBody* rb) { if (_world) _world->destroyRigidBody(rb); });
    rp3d::Collider* col =  box->addCollider(boxShape, rp3d::Transform::identity());
    box->setMass(1.0f);
    box->setLinearDamping(0.5f);
    box->setAngularDamping(1.0f);

    // 设置摩擦力和无弹性
    col->getMaterial().setFrictionCoefficient(0.8f);
    col->getMaterial().setBounciness(0.0f);

    std::shared_ptr<CollisionBody> collision = std::make_shared<CollisionBodyReactphysics3dImpl>(box);
    return collision;
}

void CollisionSystemReactphysics3dImpl::update(double timeStep) {
    if (_world) {
        _world->update(timeStep);
    }
}
