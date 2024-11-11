#ifndef _HEAD_FLAG_MODEL_BASEDEFINE_H
#define _HEAD_FLAG_MODEL_BASEDEFINE_H

#include <initializer_list>
#include <string>

struct Color {
    Color() : Color(0.0f, 0.0f, 0.0f) { }
    Color(float r, float g, float b) : r(r), g(g), b(b) { }
    Color operator* (float ratio) const { return Color(r * ratio, g * ratio, b * ratio); }

    float r;
    float g;
    float b;
};

struct XYZ {
    XYZ() : XYZ(0.0f, 0.0f, 0.0f) { }
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

inline XYZ operator*(float ratio, const XYZ& value) { return XYZ(value.x * ratio, value.y * ratio, value.z * ratio); }
inline bool operator==(const XYZ& a, const XYZ& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }

typedef XYZ Position, Vector3D, Size3D;

struct XY {
    XY() : XY(0.0f, 0.0f) { }
    XY(float x, float y) : x(x), y(y) { }

    float x;
    float y;
};

typedef XY Vector2D;

struct Matrix4X4 {
    float _data0[4] = { 0.0f };
    float _data1[4] = { 0.0f };
    float _data2[4] = { 0.0f };
    float _data3[4] = { 0.0f };
};


#endif // _HEAD_FLAG_MODEL_BASEDEFINE_H
