#include "object/Model.h"
#include "ModelMeshLoader.h"


static std::vector<Model3DVertex> MeshVertex2Vertex(const std::vector<Mesh::Vertex>& meshVertices) {
    std::vector<Model3DVertex> vertices;
    vertices.reserve(meshVertices.size());
    for (const Mesh::Vertex& meshVert : meshVertices) {
        vertices.push_back({meshVert.position, meshVert.normal, meshVert.texCoords});
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

Model3DDrawObject::Model3DDrawObject(std::string const& path) {
    load(path);
}

void Model3DDrawObject::setPosition(const Position& pos) {
    for (auto& draw : _meshDrawes) {
        draw.setPosition(pos);
    }
}

void Model3DDrawObject::setSize(const Size3D& size) {
    for (auto& draw : _meshDrawes) {
        draw.setSize(size);
    }
}

void Model3DDrawObject::setAttituedeCtrl(const Vector3D& up, const Vector3D& front) {
    for (auto& draw : _meshDrawes) {
        draw.getAttituedeCtrl()
           .setUp(up)
           .setFront(front);
    }
}

void Model3DDrawObject::show() {
    for (auto& draw : _meshDrawes) {
        draw.show();
    }
}

void Model3DDrawObject::load(const std::string& modelPath) {
    ModelMeshLoader loader;
    const std::vector<Mesh>& meshes = loader.loadModelAsMeshes(modelPath);

    for (const Mesh& mesh : meshes) {
        _meshDrawes.push_back(Model3D());
        Model3D& obj = _meshDrawes.back();

        std::map<std::string, unsigned int> textureMap = MeshTextures2TextureMap(mesh.textures);
        for (const auto& itr : textureMap) {
            obj.setTexture(itr.first, itr.second);
        }
        obj.setVertexData(MeshVertex2Vertex(mesh.vertices), mesh.indices);
    }
}

