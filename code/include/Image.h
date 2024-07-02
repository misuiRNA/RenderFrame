#ifndef _HEAD_FLAG_IMAGE_H
#define _HEAD_FLAG_IMAGE_H

#include <string>
#include <iostream> 
#include <map>
#include "BaseDefine.h"

enum class ImageWrapMode {
    Repeat      = 0,
    ClampToEdge = 1
};

using TextureId = unsigned int;

struct AbstractImage {
    virtual ~AbstractImage() { }
    virtual TextureId getTexture(ImageWrapMode wrapMode) const = 0;
};


struct LocalImage : AbstractImage {
    LocalImage(const std::string& path);
    ~LocalImage();
    int width() const;
    int height() const;
    unsigned char* data() const;
    TextureId getTexture(ImageWrapMode wrapMode) const;

private:
    int _width;
    int _height;
    unsigned int _format;
    unsigned char* _data;

    mutable std::map<ImageWrapMode, TextureId> _textureMap;
};


struct PaintImage : AbstractImage {
    PaintImage(unsigned int witdh, unsigned int height);
    void setBackgroundColor(const Color& color);
    void paint(std::function<void()> painter);
    // TODO: 优化, 离屏渲染纹理实现暂不支持选择环绕方式
    TextureId getTexture(ImageWrapMode wrapMode) const;

private:
    unsigned int _width;
    unsigned int _height;
    Color _backgroundColor;

    unsigned int _frameBuffer;
    unsigned int _texColorBuffer;
};


struct CubeImage : AbstractImage {
    static constexpr int FACE_COUNT = 6;

    CubeImage(const std::string& xRight, const std::string& xLeft, const std::string& yTop, const std::string& yBottom, const std::string& zFront, const std::string& zBack);
    ~CubeImage();
    TextureId getTexture(ImageWrapMode wrapMode) const;
private:
    LocalImage _images[FACE_COUNT];
    mutable TextureId _textureId;
};


#endif // _HEAD_FLAG_IMAGE_H
