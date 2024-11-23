#ifndef _HEAD_FLAG_UTILS_H
#define _HEAD_FLAG_UTILS_H

#include "BaseDefine.h"
#include <string>
#include <cmath>
#include <vector>
#include "RenderEngine.h"

// math units
struct MathUtils {
    static float AngleCos(float angel);
};

template<typename T>
T CubicBezierAxis(T p0, T p1, T p2, T p3 , double t)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    float result = uuu * p0;
    result += 3 * uu * t * p1;
    result += 3 * u * tt * p2;
    result += ttt * p3;

    return result;
}

template<typename TPoint>
std::vector<TPoint> CubicBezier(const TPoint& p0, const TPoint& p1, const TPoint& p2, const TPoint& p3 ,double detail)
{
    std::vector<TPoint> ret;
    if (detail <=0 || detail >= 1)
    {
        return ret;
    }
    TPoint cp0(p0.x - p0.x,
               p0.y - p0.y,
               p0.z - p0.z);
    TPoint cp1(p1.x - p0.x,
               p1.y - p0.y,
               p1.z - p0.z);
    TPoint cp2(p2.x - p0.x,
               p2.y - p0.y,
               p2.z - p0.z);
    TPoint cp3(p3.x - p0.x,
               p3.y - p0.y,
               p3.z - p0.z);
    for (float t = 0; t < 1.01; t += detail)
    {
        ret.emplace_back(TPoint(CubicBezierAxis(cp0.x, cp1.x, cp2.x, cp3.x, t),
                                CubicBezierAxis(cp0.y, cp1.y, cp2.y, cp3.y, t),
                                CubicBezierAxis(cp0.z, cp1.z, cp2.z, cp3.z, t)));
    }
    for (int i = 0; i < ret.size(); ++i)
    {
        ret[i].x += p0.x;
        ret[i].y += p0.y;
        ret[i].z += p0.z;
    }
    return ret;
}

class SpatialUtils
{
public:
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
        float cosAngle = SpatialUtils::cosAngle(vec1, vec2);
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

        float sinAngle = SpatialUtils::sinAngle(vec1, vec2);
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
        float d1 = SpatialUtils::distance(vec1);
        float d2 = SpatialUtils::distance(vec2);

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
        if (SpatialUtils::distance(unitBisVec) <= 1e-6)
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
        if (SpatialUtils::distance(unitNormal) <= 1e-6)
        {
            return p;
        }
        p.x = unitNormal.x * width;
        p.y = unitNormal.y * width;
        return p;
    }

    template<typename TPoint>
    static bool CheckSegmentIntersect(const TPoint& seg1start, const TPoint& seg1end, const TPoint& seg2start, const TPoint& seg2end)
    {

        auto orientation = [](const TPoint& p, const TPoint& q, const TPoint& r)
        {
            float cross = SpatialUtils::cross(q - p, r - p);
            if (abs(cross) <= 1e-6)
            {
                return 0;
            }
            return cross > 0.0f ? 1 : -1;
        };

        auto onSegment = [](const TPoint& p, const TPoint& q, const TPoint& r)
        {
            return (q.x <= fmax(p.x, r.x) and q.x >= fmin(p.x, r.x)
                 && q.y <= fmax(p.y, r.y) and q.y >= fmin(p.y, r.y));
        };

        TPoint p1 = seg1start;
        TPoint q1 = seg1end;
        TPoint p2 = seg2start;
        TPoint q2 = seg2end;

        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        bool isIntersect = (o1 != o2 && o3 != o4)
            || (o1 == 0 && onSegment(p1, p2, q1))
            || (o2 == 0 && onSegment(p1, q2, q1))
            || (o3 == 0 && onSegment(p2, p1, q2))
            || (o4 == 0 && onSegment(p2, q1, q2));
        return isIntersect;
    }

    template<typename TPoint>
    static void CorrectIntersectedSegments(const std::vector<TPoint>& orgPoints, std::vector<TPoint>& resPoints)
    {
        // TODO 优化: 直接检测扩展前后的线段是否平行，如果平行则不修正
        auto shouldCorrectResPoint = [](const TPoint& resPre, const TPoint& resCur, const TPoint& orgPre, const TPoint& orgCur)
        {
            if (CheckSegmentIntersect(orgPre, resPre, orgCur, resCur))
            {
                return true;
            }
            if (SpatialUtils::distance(resCur, orgCur) < 1e-6)
            {
                return true;
            }
            float resDis = SpatialUtils::distance(resPre, resCur);
            float orgDis = SpatialUtils::distance(orgPre, orgCur);
            if (MuchLarger(resDis, orgDis))
            {
                return true;
            }
            return false;
        };
        for (int index = 1; index < orgPoints.size(); index++)
        {
            if (shouldCorrectResPoint(resPoints[index - 1], resPoints[index], orgPoints[index - 1], orgPoints[index]))
            {
                resPoints[index] = resPoints[index - 1];
            }
        }
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

    template<typename TPoint>
    static void ExpandLineToAngleBisector(const std::vector<TPoint>& orgPoints, const float width, std::vector<TPoint>& leftPoints, std::vector<TPoint>& rightPoints)
    {
        std::vector<TPoint> expandVectors = CalcExpandVectorByAngleBisector(orgPoints, width);

        leftPoints.clear();
        rightPoints.clear();
        leftPoints.resize(orgPoints.size());
        rightPoints.resize(orgPoints.size());
        for (int index = 0; index < orgPoints.size(); index++)
        {
            leftPoints[index] = orgPoints[index] + expandVectors[index];
            rightPoints[index] = orgPoints[index] - expandVectors[index];
        }

        CorrectIntersectedSegments(orgPoints, leftPoints);
        CorrectIntersectedSegments(orgPoints, rightPoints);
    }

    template<typename TPoint>
    static std::vector<std::vector<int>> DetectNoisyPointBounds(const std::vector<TPoint>& points, float detectLenThreshold, float turningThreshold)
    {
        if (points.empty())
        {
            return std::vector<std::vector<int>>();
        }

        float accLenSum = 0.0f;
        std::vector<int> bigAngleIndex;
        std::vector<float> biaAngleAccLen;
        for (int index = 1; index < points.size() - 1; index++)
        {
            accLenSum += SpatialUtils::distance(points[index - 1], points[index]);
            float angle = LineUtils::CalcAngle(points[index - 1], points[index], points[index + 1]);
            if (abs(angle) < turningThreshold)
            {
                biaAngleAccLen.emplace_back(accLenSum);
                bigAngleIndex.emplace_back(index);
            }
        }
        std::vector<int> noisyIndexs;
        std::vector<std::vector<int>> noisyIndexBounds;
        for (int index = 0; index < biaAngleAccLen.size(); index++)
        {
            float distToPre = std::numeric_limits<float>::infinity();
            float distToNext = std::numeric_limits<float>::infinity();
            if (index > 0)
            {
                distToPre = std::min(distToPre, biaAngleAccLen[index] - biaAngleAccLen[index - 1]);
            }
            if (index < biaAngleAccLen.size() - 1)
            {
                distToNext = std::min(distToNext, biaAngleAccLen[index + 1] - biaAngleAccLen[index]);
            }

            bool isNoisyPoint = std::min(distToPre, distToNext) < detectLenThreshold;
            if (isNoisyPoint)
            {
                noisyIndexs.emplace_back(bigAngleIndex[index]);
                if (distToNext >= detectLenThreshold)
                {
                    noisyIndexBounds.emplace_back(noisyIndexs);
                    noisyIndexs.clear();
                }
            }
        }
        return noisyIndexBounds;
    }

    template<typename TPoint>
    static void SmoothNoisyPoints(std::vector<TPoint>& points, float detectLenThreshold, float turningThreshold)
    {
        std::vector<std::vector<int>> noisyIndexBounds = DetectNoisyPointBounds(points, detectLenThreshold, turningThreshold);

        for (int boundIndex = 0; boundIndex < noisyIndexBounds.size(); boundIndex++)
        {
            const std::vector<int>& noisyIndexs = noisyIndexBounds[boundIndex];
            if (noisyIndexs.empty() || noisyIndexs.front() < 0 || noisyIndexs.back() >= points.size())
            {
                continue;
            }
            int firstNoisyIndex = noisyIndexs.front();
            int lastNoisyIndex = noisyIndexs.back();
            int preNoisyIndex = firstNoisyIndex > 0 ? firstNoisyIndex - 1 : firstNoisyIndex;
            int afterNoisyIndex = lastNoisyIndex < points.size() - 1 ? lastNoisyIndex + 1 : lastNoisyIndex;
            int insertPointNum = lastNoisyIndex - firstNoisyIndex + 2;
            std::vector<TPoint> soomthPoints = CubicBezier(points[preNoisyIndex], points[firstNoisyIndex], points[lastNoisyIndex], points[afterNoisyIndex], 1.0 / insertPointNum);
            if (soomthPoints.empty())
            {
                continue;
            }
            for (int index = firstNoisyIndex; index <= lastNoisyIndex; index++)
            {
                int smoothIndex = index - firstNoisyIndex;
                points[index] = smoothIndex < soomthPoints.size() - 1 ? soomthPoints[smoothIndex + 1] : soomthPoints.back();
            }
        }
    }
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
