#ifndef _HEAD_FLAG_IMAGE_H
#define _HEAD_FLAG_IMAGE_H

#include <string>
#include <iostream> 

struct Image {
    Image(const std::string& path, bool isRBGA = false);
    ~Image();
    unsigned int getTexture() const;

private:
    static unsigned int genTexture(const unsigned char* data, int width, int height, unsigned int format);

private:
    bool _isRBGA;
    int _width;
    int _height;
    unsigned char* _data;

    mutable unsigned int _textureId;
};

#endif // _HEAD_FLAG_IMAGE_H
