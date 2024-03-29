#ifndef _HEAD_FLAG_IMAGE_H
#define _HEAD_FLAG_IMAGE_H

#include <string>
#include "stb_image.h"
#include <iostream> 

struct Image {
    // TODO: 优化, 自动识别isRBGA, 不要传参
    Image(const std::string& path, bool isRBGA = false) : _isRBGA(isRBGA) {
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cout << "Failed to load texture " << path << std::endl;
        }

        _width = width;
        _height = height;
        _data = data;
    }

    ~Image() {
        stbi_image_free(_data);
    }

    int width() const {
        return _width;
    }

    int height() const {
        return _height;
    }

    const unsigned char* data() const {
        return _data;
    }

    bool isRBGA() const {
        return _isRBGA;
    }

private:
    bool _isRBGA;
    int _width;
    int _height;
    unsigned char* _data;
};

#endif // _HEAD_FLAG_IMAGE_H
