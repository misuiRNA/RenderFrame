#include "model/Model3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ModelMeshLoader.h"


static std::vector<Model3DVertex> MeshVertex2Vertex(const std::vector<Mesh::Vertex>& meshVertices) {
    std::vector<Model3DVertex> vertices;
    vertices.reserve(meshVertices.size());
    for (const Mesh::Vertex& meshVert : meshVertices) {
        vertices.emplace_back(meshVert.position, meshVert.normal, meshVert.texCoords);
    }
    return vertices;
}

static std::map<std::string, unsigned int> MeshTextures2TextureMap(const std::vector<Mesh::Texture>& textures) {
    std::map<std::string, unsigned int> textureMap;
    unsigned int diffuseNr  = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr   = 0;
    unsigned int heightNr   = 0;
    for(unsigned int index = 0; index < textures.size(); index++) {
        std::string uniformName;
        switch (textures[index].type) {
            case Mesh::Texture::Type::DIFFUSE: {
                uniformName = "diffuseTexture" + ShaderProgram::UniformArraySuffix(diffuseNr++);
                break;
            }
            case Mesh::Texture::Type::SPECULAR: {
                uniformName = "specularTexture" + ShaderProgram::UniformArraySuffix(specularNr++);
                break;
            }
            case Mesh::Texture::Type::NORMAL: {
                uniformName = "normalTexture" + ShaderProgram::UniformArraySuffix(normalNr++);
                break;
            }
            case Mesh::Texture::Type::HEIGHT: {
                uniformName = "heightTexture" + ShaderProgram::UniformArraySuffix(heightNr++);
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


Model3D::Model3D(std::string const& path)
: AbstractModel(ShaderProgram::GetMeshShaderProg())
, _pos(0.0f, 0.0f, 0.0f)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f})
, _scaleRatio(1.0f)
, _modelPath(path) {
    updateModelMatrix();
    _attitudeCtrl.addOnAttitudeChangedListener([this](){ updateModelMatrix(); });
}

void Model3D::setPosition(const Position& pos) {
    _pos = pos;
    updateModelMatrix();
}

void Model3D::setScale(float scale) {
    _scaleRatio = scale;
    updateModelMatrix();
}

Attitude3DController& Model3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void Model3D::updateUniformes() {
    _renderData.setUniformMat4("model", _modelMatrix);
}

void Model3D::updateRenderData() {
    ModelMeshLoader loader;
    const std::vector<Mesh>& meshes = loader.loadModelAsMeshes(_modelPath);
    std::vector<RenderData> renderDatas;
    renderDatas.reserve(meshes.size());
    for (const Mesh& mesh : meshes) {
        RenderData data(_renderData.getShaderProgram());
        data.setVertices(MeshVertex2Vertex(mesh.vertices));
        data.setIndices(mesh.indices);
        std::map<std::string, unsigned int> textureMap = MeshTextures2TextureMap(mesh.textures);
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

void Model3D::updateModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, _pos.z));

    glm::mat4 attitudeMatrix = _attitudeCtrl.getAttitudeMatrix();
    model = model * attitudeMatrix;

    model = glm::scale(model, glm::vec3(_scaleRatio, _scaleRatio, _scaleRatio));

    memcpy(_modelMatrix, glm::value_ptr(model), sizeof(glm::mat4));
}


Model3DVertex::Model3DVertex(Vector3D pos, Vector3D normal, Vector2D texCoords)
: pos(pos)
, normal(normal)
, texCoords(texCoords) {

}

std::vector<ShaderAttribDescriptor> Model3DVertex::descriptor = {
    DESC("aPos",       0, Model3DVertex, pos),
    DESC("aNormal",    1, Model3DVertex, normal),
    DESC("aTexCoords", 2, Model3DVertex, texCoords),
};
