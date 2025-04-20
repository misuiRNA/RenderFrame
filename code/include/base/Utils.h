#ifndef _HEAD_FLAG_UTILS_H
#define _HEAD_FLAG_UTILS_H

#include "base/BaseDefine.h"
#include <string>
#include <cmath>
#include <vector>
#include "RenderEngine.h"

// math units
struct MathUtils {
    static float AngleCos(float angel);

    template<typename TPoint>
    static double dot(const TPoint &p1, const TPoint &p2)
    {
        return 1.0 * p1.x * p2.x + 1.0 * p1.y * p2.y;
    }

   template<typename TPoint>
    static double cross(const TPoint &p1, const TPoint &p2)
    {
        return 1.0 * p1.x * p2.y - 1.0 * p1.y * p2.x;
    }

    template<typename TPoint>
    static double distance(const TPoint &v)
    {
        return std::sqrt(dot(v, v));
    }

    template<typename TPoint>
    static double distance(const TPoint &p1, const TPoint &p2)
    {
        return distance(p2 - p1);
    }
    
    template<typename TPoint>
    static double sinAngle(const TPoint &v1, const TPoint &v2)
    {
        double d1 = distance(v1);
        double d2 = distance(v2);
        if (d1 < 1e-6 || d2 < 1e-6) return 0;
        return cross(v1, v2) / (1.0 * d1 * d2);
    }

    template<typename TPoint>
    static double sinAngle(const TPoint &s1p1, const TPoint &s1p2, const TPoint &s2p1, const TPoint &s2p2)
    {
        return sinAngle(s1p2 - s1p1, s2p2 - s2p1);
    }

    template<typename TPoint>
    static double cosAngle(const TPoint &v1, const TPoint &v2)
    {
        double d1 = distance(v1);
        double d2 = distance(v2);
        if (d1 < 1e-6 || d2 < 1e-6) return 0;
        return dot(v1, v2) / (1.0 * d1 * d2);
    }

    template<typename TPoint>
    static double cosAngle(const TPoint &s1p1, const TPoint &s1p2, const TPoint &s2p1, const TPoint &s2p2)
    {
        return cosAngle(s1p2 - s1p1, s2p2 - s2p1);
    }

    template<typename TPoint>
    static TPoint lerp(const TPoint &p1, const TPoint &p2, double t)
    {
        TPoint p;
        p.x = p1.x + (p2.x - p1.x) * t;
        p.y = p1.y + (p2.y - p1.y) * t;
        return p;
    }
};


struct LineUtils
{
    static constexpr float MIN_TURN_ANGLE = 5.0f;
    static constexpr float MUCH_LARGER_SCAL = 10.0f;

    static bool MuchLarger(float bigger, float smaller)
    {
        return bigger > smaller * MUCH_LARGER_SCAL;
    }

    template<typename TPoint>
    static TPoint Unit(const TPoint& normal)
    {
        float c = sqrt(normal.x * normal.x + normal.y * normal.y);
        TPoint p;
        p.x = 0.0f;
        p.y = 0.0f;
        if (c >= 1e-6)
        {
            p.x = normal.x / c;
            p.y = normal.y / c;
        }
        return p;
    }

    /********************
    * return: -180 ~ 180
    *********************/
    template<typename TPoint>
    static float CalcAngle(const TPoint& p1, const TPoint& p2, const TPoint& p3)
    {
        float angle = 0.0f;
        if (p1 == p2 || p2 == p3)
        {
            return angle;
        }

        TPoint vec1 = p2 - p1;
        TPoint vec2 = p2 - p3;
        float cosAngle = MathUtils::cosAngle(vec1, vec2);
        if (std::isnan(cosAngle))
        {
            return angle;
        }
        if (cosAngle > 1.0f)
        {
            cosAngle = 1.0f;
        }
        else if (cosAngle < -1.0f)
        {
            cosAngle = -1.0f;
        }
        angle = acos(cosAngle) * 180.0f / M_PI;

        float sinAngle = MathUtils::sinAngle(vec1, vec2);
        if (sinAngle < 0)
        {
            angle = -angle;
        }
        return angle;
    }

    template<typename TPoint>
    static TPoint CalcAngleBisector(const TPoint& p1, const TPoint& p2, const TPoint& p3, float width)
    {
        TPoint p;
        p.x = 0.0f;
        p.y = 0.0f;
        if (p1 == p2 && p2 == p3)
        {
            return p;
        }

        TPoint vec1 = p2 - p1;
        TPoint vec2 = p2 - p3;
        float d1 = MathUtils::distance(vec1);
        float d2 = MathUtils::distance(vec2);

        // 处理点特别靠近的情况
        if (MuchLarger(width, d1) && MuchLarger(width, d2))
        {
            return p;
        }
        else if (MuchLarger(width, d1))
        {
            return CalcNormal(p2, p3, width);
        }
        else if (MuchLarger(width, d2))
        {
            return CalcNormal(p1, p2, width);
        }

        TPoint unitBisVec = Unit(Unit(vec1) + Unit(vec2));
        // 处理平角
        if (MathUtils::distance(unitBisVec) <= 1e-6)
        {
            return CalcNormal(p1, p2, width);
        }

        float angle = CalcAngle(p1, p2, p3);
        if (abs(angle) <= MIN_TURN_ANGLE)
        {
            return CalcNormal(p1, p2, width);
        }
        float halfAngleSin = sin(angle / 360.0f * M_PI);
        float angleBisLen = width / halfAngleSin;

        p.x = unitBisVec.x * angleBisLen;
        p.y = unitBisVec.y * angleBisLen;
        return p;
    }

    template<typename TPoint>
    static TPoint CalcNormal(const TPoint& p1, const TPoint& p2, float width)
    {
        TPoint p;
        p.x = 0.0f;
        p.y = 0.0f;
        if (p1 == p2)
        {
            return p;
        }

        TPoint vec = p2 - p1;
        TPoint normal;
        normal.x = -vec.y;
        normal.y = vec.x;
        TPoint unitNormal = Unit(normal);
        if (MathUtils::distance(unitNormal) <= 1e-6)
        {
            return p;
        }
        p.x = unitNormal.x * width;
        p.y = unitNormal.y * width;
        return p;
    }

    template<typename TPoint>
    static std::vector<TPoint> CalcExpandVectorByAngleBisector(const std::vector<TPoint>& orgPoints, const float width)
    {
        // 法向量向左量为正
        std::vector<TPoint> expandVectors(orgPoints.size());
        if (orgPoints.size() < 2)
        {
            return expandVectors;
        }
        expandVectors[0] = CalcNormal(orgPoints[0], orgPoints[1], width);;
        for (int index = 1; index < orgPoints.size() - 1; index++)
        {
            expandVectors[index] = CalcAngleBisector(orgPoints[index - 1], orgPoints[index], orgPoints[index + 1], width);;
        }
        expandVectors[orgPoints.size() - 1] = CalcNormal(orgPoints[orgPoints.size() - 2], orgPoints[orgPoints.size() - 1], width);;
        return expandVectors;
    }

};


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
