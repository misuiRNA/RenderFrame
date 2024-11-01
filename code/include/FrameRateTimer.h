#ifndef _HEAD_FLAG_FRAMERATETIMER_H
#define _HEAD_FLAG_FRAMERATETIMER_H


struct FrameRateTimer {
    FrameRateTimer();
    float getCurTime();
    float getFrameTime();
    void updateTime();

private:
    float _currentTime;
    float _lastTime;
    float _deltaTime;
};

#endif