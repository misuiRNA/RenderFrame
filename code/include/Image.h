#ifndef _HEAD_FLAG_IMAGE_H
#define _HEAD_FLAG_IMAGE_H

#include <string>
#include <iostream> 

struct Image {
    Image(const std::string& path, bool isRBGA = false);
    ~Image();
    int width() const;
    int height() const;
    const unsigned char* data() const;
    bool isRBGA() const;

private:
    bool _isRBGA;
    int _width;
    int _height;
    unsigned char* _data;
};

#endif // _HEAD_FLAG_IMAGE_H
