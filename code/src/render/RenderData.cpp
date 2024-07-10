#include "RenderData.h"
#include "glad/glad.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


static void VAODeleter(unsigned int* VAOPtr) {
    if (*VAOPtr != 0) {
        printf("delete VAO: %d\n", *VAOPtr);
        glDeleteVertexArrays(1, VAOPtr);
        *VAOPtr = 0;
        // delete VBO
        // delete EBO
    }
    delete VAOPtr;
}

unsigned int RenderDataMode2GLMode(RenderDataMode mode) {
    static std::map<RenderDataMode, unsigned int> MODE_MAP = {
        { RenderDataMode::POINTS,         GL_POINTS },
        { RenderDataMode::LINES,          GL_LINES },
        { RenderDataMode::LINE_LOOP,      GL_LINE_LOOP },
        { RenderDataMode::LINE_STRIP,     GL_LINE_STRIP },
        { RenderDataMode::TRIANGLES,      GL_TRIANGLES },
        { RenderDataMode::TRIANGLE_FAN,   GL_TRIANGLE_FAN },
        { RenderDataMode::TRIANGLE_STRIP, GL_TRIANGLE_STRIP }
    };

    unsigned int glMode = GL_POINTS;
    if (MODE_MAP.find(mode) != MODE_MAP.end()) {
        glMode = MODE_MAP[mode];
    }
    return glMode;
}

RenderData::RenderData(ShaderProgram& prog, RenderDataMode mode)
: _prog(prog)
, _mode(RenderDataMode2GLMode(mode))
, _VAOHolder(new unsigned int (0), VAODeleter)
, _vertexCount(0)
, _indexCount(0) {

}

// remind: 浅拷贝VAO, 拷贝构造对象共用VAO
RenderData::RenderData(const RenderData& oth)
: _prog(oth._prog)
, _mode(oth._mode)
, _VAOHolder(oth._VAOHolder)
, _vertexCount(oth._vertexCount)
, _indexCount(oth._indexCount)
, _textureMap(oth._textureMap)
, _uniformFunctions(oth._uniformFunctions) {

}

RenderData::RenderData(RenderData&& oth) noexcept
: _prog(oth._prog)
, _mode(oth._mode)
, _VAOHolder(std::move(oth._VAOHolder))
, _vertexCount(oth._vertexCount)
, _indexCount(oth._indexCount)
, _textureMap(std::move(oth._textureMap))
, _uniformFunctions(std::move(oth._uniformFunctions)) {
    oth._vertexCount = 0;
    oth._indexCount = 0;
    oth._VAOHolder.reset();
}

RenderData::~RenderData() {
    // remind: VAO 等 gl 资源释放交给 _VAOHolder 管理
    _textureMap.clear();
    _uniformFunctions.clear();
}

void RenderData::setVertices(size_t vertexCount, size_t verticeStride, const void* data, const std::vector<ShaderAttribDescriptor>& descs) {
    unsigned int VBO = CreateVBO(vertexCount * verticeStride, data);

    if (VAOID() == 0) {
        glGenVertexArrays(1, _VAOHolder.get());
        // printf("alloc VAO: %d\n", VAOID());
    }
    glBindVertexArray(VAOID());
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    for (const ShaderAttribDescriptor& desc : descs)
    {
        if (verticeStride != desc.stride) {
            printf("Error: vertice buffer is not match with descriptor\n");
        }
        glVertexAttribPointer(desc.index, desc.size, GL_FLOAT, GL_FALSE, desc.stride, desc.pointer);
        glEnableVertexAttribArray(desc.index);
    }
    glBindVertexArray(0);

    // VBO绑定VAO后可以直接删除缓冲区. glDeleteBuffers实际上是标记删除, 直到所有绑定的VAO删除之后才会真正删除VBO
    glDeleteBuffers(1, &VBO);

    _vertexCount = vertexCount;
}

void RenderData::setIndices(const std::vector<unsigned int>& indices) {
    unsigned int EBO = CreateEBO(indices);

    if (VAOID() == 0)
    {
        glGenVertexArrays(1, _VAOHolder.get());
    }

    glBindVertexArray(VAOID());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);

    // VBO绑定VAO后可以直接删除缓冲区. glDeleteBuffers实际上是标记删除, 直到所有绑定的VAO删除之后才会真正删除VBO
    glDeleteBuffers(1, &EBO);
    _indexCount = indices.size();
}

void RenderData::setTexture(const std::string& name, unsigned int textureId) {
    // if (textureId == 0) {
    //     std::cout << "Failed to set texture! textureId is invalid: " << name  << " value=" << textureId << std::endl;
    //     return;
    // }
    _textureMap[name] = textureId;
}

void RenderData::setUniform(const std::string& name, int value) {
    std::function<void(ShaderProgram& prog)> func = [name, value](ShaderProgram& prog) -> void {
        prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float value) {
    std::function<void(ShaderProgram& prog)> func = [name, value](ShaderProgram& prog) -> void {
        prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float v1, float v2, float v3) {
    std::function<void(ShaderProgram& prog)> func = [name, v1, v2, v3](ShaderProgram& prog) -> void {
        prog.setUniform(name, v1, v2, v3);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, float v1, float v2, float v3, float v4) {
    std::function<void(ShaderProgram& prog)> func = [name, v1, v2, v3, v4](ShaderProgram& prog) -> void {
        prog.setUniform(name, v1, v2, v3, v4);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniformMat4(const std::string& name, const float* mat)
{
    glm::mat4 matrix = glm::make_mat4(mat);
    std::function<void(ShaderProgram& prog)> func = [name, matrix](ShaderProgram& prog) -> void {
        prog.setUniformMat4(name, glm::value_ptr(matrix));
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, const XYZ& value)
{
    std::function<void(ShaderProgram& prog)> func = [name, value](ShaderProgram& prog) -> void {
        prog.setUniform(name, value);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, const Color& color)
{
    std::function<void(ShaderProgram& prog)> func = [name, color](ShaderProgram& prog) -> void {
        prog.setUniform(name, color);
    };
    setUniformFunc(name, func);
}

void RenderData::setUniform(const std::string& name, const ShaderMaterial& material) {
    setTexture(name + ".diffuseTexture", material.diffuseTexture);
    setTexture(name + ".specularTexture", material.specularTexture);
    setUniform(name + ".ambient", material.ambient);
    setUniform(name + ".diffuse", material.diffuse);
    setUniform(name + ".specular", material.specular);
    setUniform(name + ".shininess", material.shininess);
}

void RenderData::setUniformFunc(const std::string& name, const std::function<void(ShaderProgram& prog)>& func) {
    _uniformFunctions[name] = func;
}

ShaderProgram& RenderData::getShaderProgram() const {
    return _prog;
}

void RenderData::useUniforms() {
    for (auto& pair : _uniformFunctions) {
        auto& setUniformFunc = pair.second;
        setUniformFunc(_prog);
    }
}

void RenderData::useTextures() {
    int textureSlotIdx = 0;
    for (auto& itr : _textureMap) {
        const std::string& name = itr.first;
        unsigned int textureId = itr.second;

        _prog.setUniform(name, textureSlotIdx);
        glActiveTexture(GL_TEXTURE0 + textureSlotIdx);
        glBindTexture(GL_TEXTURE_2D, textureId);
        textureSlotIdx++;
    }
}

void RenderData::resetTextures() {
    int textureSlotIdx = 0;
    for (auto& itr : _textureMap) {
        const std::string& name = itr.first;
        unsigned int textureId = itr.second;

        _prog.setUniform(name, 0);
        glActiveTexture(GL_TEXTURE0 + textureSlotIdx);
        glBindTexture(GL_TEXTURE_2D, 0);
        textureSlotIdx++;
    }
}

void RenderData::draw() {
    _prog.enable();

    useTextures();
    useUniforms();

    unsigned int vaoId = VAOID();
    if (vaoId != 0) {
        glBindVertexArray(vaoId);
        if (_indexCount > 0) {
            glDrawElements(_mode, _indexCount, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(_mode, 0, _vertexCount);
        }
        // always good practice to set everything back to defaults once configured.
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    } else {
        // std::cout << "RenderData: draw failed, VAOId is 0!" << std::endl;
    }

    resetTextures();
}


unsigned int RenderData::CreateVBO(size_t size, const void* data) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    // TODO should release VBO manual ?
    return VBO;
}

unsigned int RenderData::CreateEBO(const std::vector<unsigned int>& indices) {
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // TODO should release EBO manual ?
    return EBO;
}
