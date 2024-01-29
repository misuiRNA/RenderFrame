#ifndef _HEAD_FLAG_MODEL_RECTANGLE_H
#define _HEAD_FLAG_MODEL_RECTANGLE_H

#include "Shader.h"
#include "Image.h"

struct Color {
    Color() : Color(1.0f, 1.0f, 1.0f) { }
    Color(float r, float g, float b) : r(r), g(g), b(b) { }

    float r;
    float g;
    float b;
};

struct Rectangle {
    Rectangle(float width, float height);

    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setScaleRatio(float scaleRatio);
    void setRotation(float rotation);
    // TODO: 自动识别RGBA格式，不要手动传入
    void setImage(const std::string& filename, bool rgba = false);
    void setColor(const Color& color);

    void draw();
    
private:
    void buildModel();

private:
    // TODO: 优化 抽取出来，做成一个通用类
    ShaderProgram& _prog;
    RenderData _renderData;

    float _x;
    float _y;
    float _width;
    float _height;
    float _scaleRatio;
    float _rotation;
    Color _color;

    bool _imageEnable;

};

#endif
