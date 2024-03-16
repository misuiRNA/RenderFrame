#ifndef _HEAD_FLAG_SHADER_H
#define _HEAD_FLAG_SHADER_H

#include <string>
#include <vector>
#include <map>
#include <glad/glad.h>

struct RenderData {
    RenderData();
    RenderData(const std::map<std::string, int>& attrNameMap, const std::map<std::string, int>& textureSlotNameMap);
    ~RenderData();

    void setVertices(unsigned int index, unsigned int vertexSize, const std::vector<float>& vertices);
    void setVertices(const std::string& name, unsigned int vertexSize, const std::vector<float>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    void setTexture(unsigned int slotIndex, unsigned int width, unsigned int height, const unsigned char* imageData, unsigned int format = GL_RGB);
    void setTexture(const std::string& name, unsigned int width, unsigned int height, const unsigned char* imageData, unsigned int format = GL_RGB);

    void draw();

private:
    unsigned int _VAOId;

    int _vertexCount;
    int _indexCount;

    const std::map<std::string, int>& _attrNameMap;
    const std::map<std::string, int>& _textureSlotNameMap;
    std::map<int, unsigned int> _textureMap;
};

struct ShaderProgram {
    ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr);
    ShaderProgram(const std::string& vsShaderCodeStr, const std::string& fsShaderCodeStr, const std::map<std::string, int>& attrNameMap, const std::map<std::string, int>& textureSlotNameMap);

    // TODO 根据需要重载setUniform函数
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniformMat4(const std::string& name, const float* mat);
    RenderData getRenderData() const;

    void draw(RenderData& attribute);

public:
    static ShaderProgram& getRectShaderProg();
    static ShaderProgram& getCuboidShaderProg();
    static std::map<ShaderProgram*, int>& getAllShaderProg();

private:
    static unsigned int BuildShader(const char* shaderCode, unsigned int shaderType);

private:
    unsigned int _progId;
    const RenderData _renderDataPrototype;

    std::map<std::string, int> _attrNameMap;
    std::map<std::string, int> _textureSlotNameMap;

    static std::map<ShaderProgram*, int> _registProgramMap;
};

std::string GetCurPath();
std::string ReadFile(const std::string& path);

#endif // #_HEAD_FLAG_SHADER_H
