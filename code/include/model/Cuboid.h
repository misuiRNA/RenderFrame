#ifndef _HEAD_FLAG_MODEL_CUBOID_H
#define _HEAD_FLAG_MODEL_CUBOID_H


#include "Shader.h"
#include "Image.h"

struct Cubiod {
    Cubiod(float size_x, float size_y, float size_z);

    void setPosition(float x, float y, float z);
    void setSize(float size_x, float size_y, float size_z);
    void setScaleRatio(float scaleRatio);
    void setRotation(float rotation);
    void setRotationAxis(const float (&axis)[3]);
    // TODO: 自动识别RGBA格式，不要手动传入
    void addImage(const std::string& filename, bool rgba = false);

    void draw();
    
private:
    void buildModel();

private:
    static ShaderProgram& getRectShaderProg();

private:
    // TODO: 优化 抽取出来，做成一个通用类
    ShaderProgram& _prog;
    RenderData _renderData;

    float _x;
    float _y;
    float _z;
    float _size_x;
    float _size_y;
    float _size_z;
    float _scaleRatio;
    float _rotation;
    float _rotationAxis[3];

    int _imageCount;

private:
    static const std::string MODEL_NAME;
    static const std::string VS_SHADER_STR;
    static const std::string FS_SHADER_STR;
    static const std::map<std::string, int> TEXTURE_SLOT_NAME_MAP;
    static const std::map<std::string, int> ATTRIBUTE_NAME_MAP;

};

#endif