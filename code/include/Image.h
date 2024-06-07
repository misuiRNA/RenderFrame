#ifndef _HEAD_FLAG_IMAGE_H
#define _HEAD_FLAG_IMAGE_H

#include <string>
#include <iostream> 
#include <map>

enum class TextureWrapMode {
    Repeat      = 0,
    ClampToEdge = 1
};

struct Image {
    using TextureId = unsigned int;

    Image(const std::string& path);
    ~Image();
    TextureId getTexture(TextureWrapMode wrapMode) const;

private:
    int _width;
    int _height;
    unsigned int _format;
    unsigned char* _data;

    mutable std::map<TextureWrapMode, TextureId> _textureMap;
};


// TODO: 优化, 为 Canva 和 Image 抽取父类, 形成接口
struct Canva {
    using TextureId = unsigned int;

    Canva();
    void paint(std::function<void()> painter);
    TextureId getTexture() const;


    unsigned int _frameBuffer;
    unsigned int _texColorBuffer;
};


#endif // _HEAD_FLAG_IMAGE_H
