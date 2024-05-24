#ifndef _HEAD_FLAG_UTILS_H
#define _HEAD_FLAG_UTILS_H

#include "BaseDefine.h"
#include <string>

// math units
struct MathUtils {
    static float AngleCos(float angel);
};


// file system utils
std::string GetCurPath();
std::string ReadFile(const std::string& path);

#endif // _HEAD_FLAG_UTILS_H
