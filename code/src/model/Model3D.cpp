#include "model/Model3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model3DLoader.h"

Model3D::Model3D(std::string const& path)
: AbstractModel(ShaderProgram::getMeshShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _front(1.0f, 0.0f, 0.0f)
, _up(0.0f, 0.0f, 1.0f)
, _scaleRatio(1.0f)
, _modelPath(path) {

}

void Model3D::setPosition(const Position& pos) {
    _pos = pos;
}

void Model3D::setFront(const Vector3D& front) {
    _front = front;
}

void Model3D::setUp(const Vector3D& up) {
    _up = up;
}

void Model3D::setScale(float scale) {
    _scaleRatio = scale;
}

void Model3D::updateUniformes() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));

    // TODO: 优化, 需要保证上向量和前向量不平行, 否则旋转矩阵无效
    glm::vec3 normalUp = glm::vec3(_up.x, _up.y, _up.z);
    glm::vec3 normalFront = glm::normalize(glm::vec3(_front.x, _front.y, _front.z));
    glm::vec3 normalRight = glm::normalize(glm::cross(normalUp, normalFront));
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix[0] = glm::vec4(normalRight, 0.0f);
    rotationMatrix[1] = glm::vec4(normalUp,    0.0f);
    rotationMatrix[2] = glm::vec4(normalFront, 0.0f);
    rotationMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    model = model * rotationMatrix;
    model = glm::scale(model, glm::vec3(_scaleRatio, _scaleRatio, _scaleRatio));

    _renderData.setUniformMat4("model", glm::value_ptr(model));
}

static std::map<std::string, unsigned int> TextureList2Map(const std::vector<Texture>& textures) {
    std::map<std::string, unsigned int> textureMap;
    unsigned int diffuseNr  = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr   = 0;
    unsigned int heightNr   = 0;
    for(unsigned int index = 0; index < textures.size(); index++) {
        std::string uniformName;
        switch (textures[index].type) {
            case Texture::Type::DIFFUSE: {
                uniformName = ShaderProgram::UniformArrayName("diffuseTexture", diffuseNr++);
                break;
            }
            case Texture::Type::SPECULAR: {
                uniformName = ShaderProgram::UniformArrayName("specularTexture", specularNr++);
                break;
            }
            case Texture::Type::NORMAL: {
                uniformName = ShaderProgram::UniformArrayName("normalTexture", normalNr++);
                break;
            }
            case Texture::Type::HEIGHT: {
                uniformName = ShaderProgram::UniformArrayName("heightTexture", heightNr++);
                break;
            }
            default:
                // error log
                break;
        }
        textureMap[uniformName] = textures[index].id;
    }
    return textureMap;
}

void Model3D::updateRenderData() {
    Model3DLoader loader;
    const std::vector<Mesh>& meshes = loader.loadModel(_modelPath);
    std::vector<RenderData> renderDatas;
    renderDatas.reserve(meshes.size());
    for (const Mesh& mesh : meshes) {
        RenderData data(_renderData.getShaderProgram());
        data.setVertices(mesh.vertices);
        data.setIndices(mesh.indices);
        std::map<std::string, unsigned int> textureMap = TextureList2Map(mesh.textures);
        for (const auto& itr : textureMap) {
            data.setTexture(itr.first, itr.second);
        }
        renderDatas.push_back(std::move(data));
    }
    _meshRenderDatas = std::move(renderDatas);
}

void Model3D::doDraw() {
    _renderData.draw();
    for (RenderData& data : _meshRenderDatas) {
        data.draw();
    }
}


std::vector<ShaderAttribDescriptor> Vertex::descriptor = {
    {0, 3, (void*)offsetof(Vertex, position)},
    {1, 3, (void*)offsetof(Vertex, normal)},
    {2, 2, (void*)offsetof(Vertex, texCoords)},
    {3, 3, (void*)offsetof(Vertex, tangent)},
    {4, 3, (void*)offsetof(Vertex, bitangent)},
    {5, 4, (void*)offsetof(Vertex, boneIds)},
    {6, 4, (void*)offsetof(Vertex, weights)},
};
