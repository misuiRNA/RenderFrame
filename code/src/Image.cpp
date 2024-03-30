#include "Image.h"
#include "stb_image.h"

// TODO: 优化, 自动识别isRBGA, 不要传参
Image::Image(const std::string& path, bool isRBGA) : _isRBGA(isRBGA) {
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

Image::~Image() {
    stbi_image_free(_data);
}

int Image::width() const {
    return _width;
}

int Image::height() const {
    return _height;
}

const unsigned char* Image::data() const {
    return _data;
}

bool Image::isRBGA() const {
    return _isRBGA;
}


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
