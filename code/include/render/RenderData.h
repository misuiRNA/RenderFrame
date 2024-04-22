#ifndef _HEAD_FLAG_RENDERDATA_H
#define _HEAD_FLAG_RENDERDATA_H

#include <vector>
#include <map>
#include <string>
#include <functional>
#include "BaseDefine.h"
#include "ShaderProgram.h"
#include "ShaderMaterial.h"
#include "glad/glad.h"


struct ShaderAttribDescriptor {
    ShaderAttribDescriptor(unsigned int index, unsigned int size, const void* pointer) : index(index), size(size), pointer(pointer) { }

    unsigned int index;
    unsigned int size;
    const void* pointer;
};


struct RenderData {
    RenderData(ShaderProgram& prog);
    ~RenderData();
    RenderData& operator= (const RenderData& oth) {
        _prog = oth._prog;
        _VAOId = oth._VAOId;
        _vertexCount = oth._vertexCount;
        _indexCount = oth._indexCount;
        // _needFreeVAOSelf = oth._needFreeVAOSelf;
        _children = oth._children;
        _textureMap = oth._textureMap;
        _uniformFunctions = oth._uniformFunctions;
        _textureFunctions = oth._textureFunctions;
        return *this;
    }

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
    void setUniform(const std::string& name, const ShaderMaterial& material);
    void setUniformMat4(const std::string& name, const float* mat);

    void setChildren(const std::vector<RenderData>& children);
    RenderData genChild();

    void draw();

private:
    void useTextures();
    void useUniforms();
    void drawAttributes();
    void setUniformFunc(const std::string& name, const std::function<void(void)>& func);
    void setTextureFunc(const std::string& name, const std::function<void(void)>& func);

public:

    template <typename T>
    void setVertices(const std::vector<T>& vertices) {
        if (_VAOId == 0) {
            glGenVertexArrays(1, &_VAOId);
        }
        glBindVertexArray(_VAOId);

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), &vertices[0], GL_STATIC_DRAW);
        for (const ShaderAttribDescriptor& desc : T::descriptor) {
            glVertexAttribPointer(desc.index, desc.size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)desc.pointer);
            glEnableVertexAttribArray(desc.index);
        }

        _vertexCount = vertices.size();
        glBindVertexArray(0);
    }


private:
    ShaderProgram& _prog;
    unsigned int _VAOId;

    int _vertexCount;
    int _indexCount;
    bool _needFreeVAOSelf = false;    // TODO: 优化, 临时方案, 需要进一步考虑VAO与RenderData的关系

    std::vector<RenderData> _children;
    std::map<std::string, int> _textureMap;
    std::map<std::string, std::function<void(void)>> _uniformFunctions;
    std::map<std::string, std::function<void(void)>> _textureFunctions;
};

#endif // _HEAD_FLAG_RENDERDATA_H
