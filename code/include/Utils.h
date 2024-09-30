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


// collection utils

template <typename Src, typename Target>
std::vector<Target> ConvertList(const std::vector<Src>& srcList, std::function<Target(const Src&)> convert) {
    std::vector<Target> dstList;
    dstList.reserve(srcList.size());
    for (const auto& item : srcList) {
        dstList.push_back(convert(item));
    }
    return dstList;
}


#endif // _HEAD_FLAG_UTILS_H
