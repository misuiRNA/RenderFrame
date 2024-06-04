#include "Image.h"
#include "stb_image.h"
#include "glad/glad.h"

static unsigned int WrapMode2GL(TextureWrapMode wrapMode) {
    unsigned int glWrapMode = GL_REPEAT;
    switch (wrapMode) {
        case TextureWrapMode::Repeat: {
            glWrapMode = GL_REPEAT;
            break;
        }
        case TextureWrapMode::ClampToEdge: {
            glWrapMode = GL_CLAMP_TO_EDGE;
        }
        default: {
            break;
        }
    }
    return glWrapMode;
}

static unsigned int GenTexture(const unsigned char* imageData, int width, int height, unsigned int format, unsigned int wrapMode) {
    if (!imageData) {
        std::cout << "Failed to set texture! imageData is null " << std::endl;
        return 0;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // TODO: should release texture manual ?

    return texture;
}

Image::Image(const std::string& path)
: _width(0)
, _height(0)
, _format(GL_RGB) {
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

unsigned int Image::getTexture(TextureWrapMode wrapMode) const {
    auto it = _textureMap.find(wrapMode);
    if (it == _textureMap.end()) {
        unsigned int glWrapMode = WrapMode2GL(wrapMode);
        _textureMap[wrapMode] = GenTexture(_data, _width, _height, _format, glWrapMode);
    }
    return _textureMap[wrapMode];
}


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
