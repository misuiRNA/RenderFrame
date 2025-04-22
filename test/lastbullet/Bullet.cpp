#include "Bullet.h"
#include "assets/UtilsFileSys.h"


static LocalImage awesomefaceImage(GetCurPath() + "/resource/awesomeface.png");

extern RenderShape cubeShape;

Bullet::Bullet(const Position& pos, const Vector3D& front)
: _shader(Size3D(0.5f, 0.5f, 0.5f))
, _speed(0) {
    _shader.setVertexData(RenderShapeAdapter<ColorTexMulilight3DShader>(cubeShape));
    _shader.setPrimaryImage(awesomefaceImage);
    _shader.setPosition(pos);
    _shader.getTransform().setFront(front);
}

void Bullet::setPosition(const Position& pos) {
    _shader.setPosition(pos);
}

void Bullet::enableCollision(bool enable) {
    if (!enable) {
        _collision.reset();
        return;
    }
    Transform trans = _shader.getTransform();
    _collision = CollisionSystem::getInstance().createCollisionBody(trans);
    // _collision->setSpeed(_speed);
}

void Bullet::setSpeed(float speed) { 
    _speed = speed;
    if (_collision) {
        _collision->setSpeed(speed);
    }
 }

void Bullet::doAmination() {
    if (!_collision) {
        return;
    }

    Transform trans = _collision->getTransform();
    _shader.setPosition(trans.getPosition());
    // _shader.getTransform().setFront(attr.getFront());
}

const Position& Bullet::getPosition() {
    return _shader.getTransform().getPosition();
}

const Vector3D& Bullet::getFront() {
    return _shader.getTransform().getFront();
}

void Bullet::show() {
    _frameTimer.updateTime();
    doAmination();
    _shader.show();
}
