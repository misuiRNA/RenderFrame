#include "KeyboardEventHandler.h"


static unsigned long long GenEventMapKey(int keyCode, int actionCode) {
    unsigned long long keyHight = keyCode;
    unsigned long long eventKey = keyHight << 32 | actionCode;
    return eventKey;
}

static int GetKeyCode(unsigned long long eventKey) {
    int keyCode = eventKey >> 32 & 0xFFFFFFFF;
    return keyCode;
}

static int GetActionCode(unsigned long long eventKey) {
    int eventCode = eventKey & 0xFFFFFFFF;
    return eventCode;
}


KeyboardEventHandler::KeyboardEventHandler(const KeyboardEventDetector detector)
: _keyboardEventDetector(detector) {

}

void KeyboardEventHandler::exrcute() {
    for (auto itr : _observersMap) {
        int keyCode = GetKeyCode(itr.first);
        int actionCode = GetActionCode(itr.first);
        if (_keyboardEventDetector(keyCode, actionCode)) {
            const std::vector<KeyboardEventObserver>& observers = itr.second;
            for (const KeyboardEventObserver& observer : observers) {
                observer();
            }
        }
    }
}

void KeyboardEventHandler::registerObserver(int keyCode, int actionCode, const KeyboardEventObserver& observer) {
    unsigned long long eventKey = GenEventMapKey(keyCode, actionCode);
    _observersMap[eventKey].push_back(observer);
}
