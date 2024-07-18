#ifndef _HEAD_FLAG_KEYBOARDEVENTHANDLER_H
#define _HEAD_FLAG_KEYBOARDEVENTHANDLER_H

#include <vector>
#include <map>
#include <functional>

using KeyboardEventDetector = std::function<bool(int keyCode, int actionCode)>;
using KeyboardEventObserver = std::function<void(void)>;

struct KeyboardEventHandler {
    KeyboardEventHandler(const KeyboardEventDetector detector);
    void exrcute();
    void registerObserver(int keyCode, int actionCode, const KeyboardEventObserver& observer);

private:
    const KeyboardEventDetector _keyboardEventDetector;
    std::map<unsigned long long, std::vector<KeyboardEventObserver>> _observersMap;
};

#endif // _HEAD_FLAG_KEYBOARDEVENTHANDLER_H
