#include "KeyboardEventHandler.h"


static unsigned long long GenEventMapKey(int keyCode, int eventCode) {
    unsigned long long keyHight = keyCode;
    unsigned long long eventMapKey = keyHight << 32 | eventCode;
    return eventMapKey;
}

static int GetKeyCode(unsigned long long eventMapKey) {
    int keyCode = eventMapKey >> 32 & 0xFFFFFFFF;
    return keyCode;
}

static int GetEventCode(unsigned long long eventMapKey) {
    int eventCode = eventMapKey & 0xFFFFFFFF;
    return eventCode;
}


KeyboardEventHandler::KeyboardEventHandler(const KeyboardEventDetector detector)
: _keyboardEventDetector(detector) {

}

void KeyboardEventHandler::proc() {
    for (auto itr : _observersMap) {
        int keyCode = GetKeyCode(itr.first);
        int eventCode = GetEventCode(itr.first);
        if (_keyboardEventDetector(keyCode, eventCode)) {
            const std::vector<KeyboardEventObserver>& observers = itr.second;
            for (const KeyboardEventObserver& observer : observers) {
                observer();
            }
        }
    }
}

void KeyboardEventHandler::registerObserver(int keyCode, int eventCode, const KeyboardEventObserver& observer) {
    unsigned long long eventMapKey = GenEventMapKey(keyCode, eventCode);
    _observersMap[eventMapKey].push_back(observer);
}
