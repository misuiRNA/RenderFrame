#include "Image.h"
#include "stb_image.h"
#include "glad/glad.h"

// TODO: 优化, 自动识别isRBGA, 不要传参
Image::Image(const std::string& path)
: _width(0)
, _height(0)
, _format(GL_RGB)
, _textureId(0) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture " << path << std::endl;
    }

    if (nrChannels == 1) {
        _format = GL_RED;
    }
    else if (nrChannels == 3) {
        _format = GL_RGB;
    }
    else if (nrChannels == 4) {
        _format = GL_RGBA;
    }

    _width = width;
    _height = height;
    _data = data;
}

Image::~Image() {
    stbi_image_free(_data);
}

unsigned int Image::getTexture() const {
    if (_textureId == 0) {
        _textureId = genTexture(_data, _width, _height, _format);
    }
    return _textureId;
}

unsigned int Image::genTexture(const unsigned char* imageData, int width, int height, unsigned int format) {
    if (!imageData) {
        std::cout << "Failed to set texture! imageData is null " << std::endl;
        return 0;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // TODO: should release texture manual ?

    return texture;
}

Image::operator TextureId() const {
    return getTexture();
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
