#include "FrameRateTimer.h"
#include <GLFW/glfw3.h>

FrameRateTimer::FrameRateTimer()
: _currentTime(0.0f)
, _lastTime(0.0f)
, _deltaTime(0.0f) {

}

double FrameRateTimer::getCurTime() {
    return _currentTime;
}

double FrameRateTimer::getFrameTime() {
    return _deltaTime;
}

void FrameRateTimer::updateTime() {
    if (_currentTime <= 1e-6)
    {
        _currentTime = glfwGetTime();
    }
    _lastTime = _currentTime;
    _currentTime = glfwGetTime();
    _deltaTime = _currentTime - _lastTime;
}

