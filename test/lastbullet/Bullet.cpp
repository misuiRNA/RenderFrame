#include "Bullet.h"


static LocalImage awesomefaceImage(GetCurPath() + "/resource/awesomeface.png");

extern RenderShape cubeShape;

Bullet::Bullet(const Position& pos, const Vector3D& front)
: _shader(Size3D(0.5f, 0.5f, 0.5f))
, _speed(10) {
    _shader.setVertexData(cubeShape);
    _shader.setPrimaryImage(awesomefaceImage);
    _shader.setPosition(pos);
    _shader.getAttituedeCtrl().setFront(front);
}

void Bullet::setPosition(const Position& pos) {
    _shader.setPosition(pos);
}

void Bullet::enableCollision(bool enable) {
    if (!enable) {
        _collision.reset();
        return;
    }
    Attitude3DController atti = _shader.getAttituedeCtrl();
    _collision = CollisionSystem::getInstance().createCollisionBody(atti);
    _collision->setSpeed(_speed);
}

void Bullet::doAmination() {
    if (!_collision) {
        return;
    }

    Attitude3DController atti = _collision->getAttituedeCtrl();
    _shader.setPosition(atti.getPosition());
    // _shader.getAttituedeCtrl().setFront(attr.getFront());
}

const Position& Bullet::getPosition() {
    return _shader.getAttituedeCtrl().getPosition();
}

const Vector3D& Bullet::getFront() {
    return _shader.getAttituedeCtrl().getFront();
}

void Bullet::show() {
    _frameTimer.updateTime();
    doAmination();
    _shader.show();
}
