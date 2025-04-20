#include "FrameRateTimer.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>


static const int FPS = 30;

FrameRateTimer::FrameRateTimer()
: _lastTime(0.0f)
, _deltaTime(0.0f) {

}

double FrameRateTimer::getCurTime() {
    return _lastTime;
}

double FrameRateTimer::getFrameTime() {
    return _deltaTime;
}

void FrameRateTimer::updateTime() {
    const float FPS_TIME = 1.0f / FPS;

    double diff = glfwGetTime() - _lastTime;
    if (diff < FPS_TIME)
    {
        double sleepTime = FPS_TIME - diff;
        std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
    }
    double curTime = glfwGetTime();
    if (_lastTime > 1e-1) {
        _deltaTime = curTime - _lastTime;
    }
    _lastTime = curTime;
}

