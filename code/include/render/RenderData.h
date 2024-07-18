#ifndef _HEAD_FLAG_RENDERDATA_H
#define _HEAD_FLAG_RENDERDATA_H

#include <vector>
#include <map>
#include <string>
#include <functional>
#include "BaseDefine.h"
#include "ShaderProgram.h"
#include "ShaderMaterial.h"
#include <shared_mutex>
#include <iostream>


enum class RenderDataMode {
    POINTS          = 0,
    LINES           = 1,
    LINE_LOOP       = 2,
    LINE_STRIP      = 3,
    TRIANGLES       = 4,
    TRIANGLE_STRIP  = 5,
    TRIANGLE_FAN    = 6
};


struct RenderData {
    RenderData(ShaderProgram& prog, RenderDataMode mode);
    RenderData(const RenderData& oth);
    RenderData(RenderData&& oth) noexcept;    // remind: 声明为 noexcept 系统才会优先使用移动构造函数
    ~RenderData();
    RenderData& operator=(RenderData&& oth) = delete;

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

    ShaderProgram& getShaderProgram() const;
    void draw();

private:
    void setVertices(size_t vertexCount, size_t verticeStride, const void* data, const std::vector<ShaderAttribDescriptor>& descs);
    void setInstanceVertices(size_t vertexCount, size_t verticeStride, const void* data, const std::vector<ShaderAttribDescriptor>& descs);
    void useTextures();
    void resetTextures();
    void useUniforms();
    void setUniformFunc(const std::string& name, const std::function<void(ShaderProgram& prog)>& func);

    unsigned int VAOID() const { return *_VAOHolder; }
    bool isElementDrawMode() const { return _indexCount > 0; }
    bool isInstanceDrawMode() const { return _instanceCount > 0; }

private:
    static unsigned int CreateVBO(size_t size, const void* data);
    static unsigned int CreateEBO(const std::vector<unsigned int>& indices);

public:

    template<typename T>
    void setVertices(const std::string& name, const std::vector<T>& vertices) {
        if (!_prog.checkVertice(name)) {
            std::cout << "Failed to set attribute! invalid vertex name!" << name << std::endl;
            return;
        }

        // TODO: 优化 ShaderAttribDescriptor.size 计算方式, T不一定完全是float组成的
        std::vector<ShaderAttribDescriptor> descs = {{name, _prog.getVerticeSlotId(name), sizeof(T) / sizeof(float), sizeof(T), (void*)0}};
        setVertices(vertices.size(), sizeof(T), vertices.data(), descs);
    }

    template <typename T>
    void setVertices(const std::vector<T>& vertices) {
        setVertices(vertices.size(), sizeof(T), vertices.data(), _prog.getVertexDescriptors());
    }

    // TODO: 优化, 改名字 突出多实例差异化顶点的特征
    template <typename T>
    void setInstanceVertices(const std::vector<T>& vertices, const std::vector<ShaderAttribDescriptor>& descs) {
        setInstanceVertices(vertices.size(), sizeof(T), vertices.data(), descs);
    }


private:
    ShaderProgram& _prog;
    const unsigned int _mode;
    std::shared_ptr<unsigned int> _VAOHolder;

    int _vertexCount;
    int _indexCount;

    int _instanceCount;

    std::map<std::string, int> _textureMap;
    std::map<std::string, std::function<void(ShaderProgram& prog)>> _uniformFunctions;
};

#endif // _HEAD_FLAG_RENDERDATA_H
