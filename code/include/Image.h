#ifndef _HEAD_FLAG_IMAGE_H
#define _HEAD_FLAG_IMAGE_H

#include <string>
#include <iostream> 

struct Image {
    using TextureId = unsigned int;

    Image(const std::string& path, bool isRBGA = false);
    ~Image();
    TextureId getTexture() const;
    operator TextureId() const;

private:
    static TextureId genTexture(const unsigned char* data, int width, int height, unsigned int format);

private:
    bool _isRBGA;
    int _width;
    int _height;
    unsigned char* _data;

    mutable TextureId _textureId;
};

#endif // _HEAD_FLAG_IMAGE_H
