#ifndef _HEAD_FLAG_RENDERDATA_H
#define _HEAD_FLAG_RENDERDATA_H

#include <vector>
#include <map>
#include <string>
#include <functional>
#include "BaseDefine.h"
#include "ShaderProgram.h"


struct RenderData {
    RenderData(ShaderProgram& prog);
    ~RenderData();

    void setVertices(unsigned int index, unsigned int vertexSize, const std::vector<float>& vertices);
    void setVertices(const std::string& name, unsigned int vertexSize, const std::vector<float>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    void setTexture(const std::string& name, unsigned int textureId);

    // TODO 根据需要重载setUniform函数
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, float v1, float v2, float v3);
    void setUniform(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniform(const std::string& name, const XYZ& value);
    void setUniform(const std::string& name, const Color& color);
    void setUniformMat4(const std::string& name, const float* mat);

    void draw();

private:
    void useTextures();
    void useUniforms();
    void drawAttributes();

private:
    ShaderProgram& _prog;
    unsigned int _VAOId;

    int _vertexCount;
    int _indexCount;

    std::map<std::string, int> _textureMap;
    std::map<std::string, std::function<void(void)>> _uniformFunctions;
    std::map<std::string, std::function<void(void)>> _textureFunctions;
};

#endif // _HEAD_FLAG_RENDERDATA_H
