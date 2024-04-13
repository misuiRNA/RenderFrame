#ifndef _HEAD_FLAG_MODEL_BASEDEFINE_H
#define _HEAD_FLAG_MODEL_BASEDEFINE_H

#include <initializer_list>

struct Color {
    Color() : Color(0.0f, 0.0f, 0.0f) { }
    Color(float r, float g, float b) : r(r), g(g), b(b) { }
    Color operator* (float ratio) const { return Color(r * ratio, g * ratio, b * ratio); }

    float r;
    float g;
    float b;
};

struct XYZ {
    XYZ(float x, float y, float z) : x(x), y(y), z(z) { }
    XYZ(std::initializer_list<double> list) {
        auto it = list.begin();
        if (it != list.end()) x = *it++; else x = 0.0;
        if (it != list.end()) y = *it++; else y = 0.0;
        if (it != list.end()) z = *it;   else z = 0.0;
    }
    XYZ operator-(const XYZ& oth) const { return XYZ(x - oth.x, y - oth.y, z - oth.z); }
    XYZ operator+(const XYZ& oth) const { return XYZ(x + oth.x, y + oth.y, z + oth.z); }

    float x;
    float y;
    float z;
};

typedef XYZ Position, Vector3D, Size3D;


// math units

struct MathUtils {
    static float AngleCos(float angel);
};

#endif // _HEAD_FLAG_MODEL_BASEDEFINE_H
