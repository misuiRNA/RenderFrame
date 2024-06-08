#include "Image.h"
#include "stb_image.h"
#include "glad/glad.h"

static unsigned int WrapMode2GL(ImageWrapMode wrapMode) {
    unsigned int glWrapMode = GL_REPEAT;
    switch (wrapMode) {
        case ImageWrapMode::Repeat: {
            glWrapMode = GL_REPEAT;
            break;
        }
        case ImageWrapMode::ClampToEdge: {
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

LocalImage::LocalImage(const std::string& path)
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

LocalImage::~LocalImage() {
    stbi_image_free(_data);
}

TextureId LocalImage::getTexture(ImageWrapMode wrapMode) const {
    auto it = _textureMap.find(wrapMode);
    if (it == _textureMap.end()) {
        unsigned int glWrapMode = WrapMode2GL(wrapMode);
        _textureMap[wrapMode] = GenTexture(_data, _width, _height, _format, glWrapMode);
    }
    return _textureMap[wrapMode];
}


static unsigned int CreateFrameBuffer() {
    unsigned int frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    return frameBuffer;
}

static unsigned int CreateTextureColorBuffer(unsigned int width, unsigned int height) {
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texColorBuffer;
}

static unsigned int CreateRenderBuffer(unsigned int width, unsigned int height) {
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return rbo;
}

PaintImage::PaintImage(unsigned int witdh, unsigned int height)
: _width(witdh)
, _height(height)
, _backgroundColor(0.0f, 0.0f, 0.0f)
, _frameBuffer(0)
, _texColorBuffer(0) {
    _frameBuffer = CreateFrameBuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    // TODO: release frame buffer after use

    _texColorBuffer = CreateTextureColorBuffer(_width, _height);
    // 将它附加到当前绑定的帧缓冲对象
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texColorBuffer, 0);
    // TODO: release texture after use

    unsigned int rbo = CreateRenderBuffer(_width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    // TODO: release rbo after use

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PaintImage::setBackgroundColor(const Color& color) { 
    _backgroundColor = color;
}

void PaintImage::paint(std::function<void()> painter) {
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, 1.0f);

    painter();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureId PaintImage::getTexture(ImageWrapMode wrapMode) const {
    return _texColorBuffer;
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
