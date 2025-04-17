#ifndef _HEAD_FLAG_FRAMERATETIMER_H
#define _HEAD_FLAG_FRAMERATETIMER_H


struct FrameRateTimer {
    FrameRateTimer();
    double getCurTime();
    double getFrameTime();
    void updateTime();

private:
    double _currentTime;
    double _lastTime;
    double _deltaTime;
};

#endif