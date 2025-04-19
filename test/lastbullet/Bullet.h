#ifndef _HEAD_FLAG_BULLET_H
#define _HEAD_FLAG_BULLET_H

#include "shader/ColorTexMulilight3DShader.h"
#include "FrameRateTimer.h"
#include "collision/Collision.h"


struct Bullet {
    Bullet(const Position& pos, const Vector3D& front);
    void setPosition(const Position& pos);
    void show();
    void enableCollision(bool enable);
    
    const Position& getPosition();
    const Vector3D& getFront();

private:
    void doAmination();

private:
    ColorTexMulilight3DShader _shader;
    FrameRateTimer _frameTimer;
    float _speed;    // 初始移动速度 单位 距离/s

    std::shared_ptr<CollisionBody> _collision;
};

#endif