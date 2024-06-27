#ifndef _HEAD_FLAG_KEYBOARDEVENTHANDLER_H
#define _HEAD_FLAG_KEYBOARDEVENTHANDLER_H

#include <vector>
#include <map>
#include <functional>

using KeyboardEventObserver = std::function<void(void)>;
using KeyboardEventDetector = std::function<bool(int keyCode, int eventCode)>;

struct KeyboardEventHandler {
    KeyboardEventHandler(const KeyboardEventDetector detector);
    void proc();
    void registerObserver(int keyCode, int eventCode, const KeyboardEventObserver& observer);

private:
    const KeyboardEventDetector _keyboardEventDetector;
    std::map<unsigned long long, std::vector<KeyboardEventObserver>> _observersMap;
};

#endif // _HEAD_FLAG_KEYBOARDEVENTHANDLER_H
