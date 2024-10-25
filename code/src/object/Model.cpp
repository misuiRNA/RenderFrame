#include "object/Model.h"
#include "ModelMeshLoader.h"
#include "Utils.h"


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
        _meshDrawes.push_back(Model3DShader());
        Model3DShader& obj = _meshDrawes.back();

        std::function<Model3DVertex(const Mesh::Vertex&)> convert = [](const Mesh::Vertex& v) -> Model3DVertex { return {v.position, v.normal, v.texCoords}; };
        std::vector<Model3DVertex> vertices = ConvertList(mesh.vertices, convert);
        obj.setVertexData(vertices, mesh.indices);

        unsigned int diffuseNr  = 0;
        unsigned int specularNr = 0;
        unsigned int normalNr   = 0;
        unsigned int heightNr   = 0;
        for(unsigned int index = 0; index < mesh.textures.size(); index++) {
            const Mesh::Texture& texture = mesh.textures[index];
            switch (texture.type) {
                case Mesh::Texture::Type::DIFFUSE: {
                    obj.setDiffuseImage(diffuseNr++, texture.image);
                    break;
                }
                case Mesh::Texture::Type::SPECULAR: {
                    obj.setSpecularImage(specularNr++, texture.image);
                    break;
                }
                case Mesh::Texture::Type::NORMAL: {
                    obj.setNormalImage(normalNr++, texture.image);
                    break;
                }
                case Mesh::Texture::Type::HEIGHT: {
                    obj.setHeightImage(heightNr++, texture.image);
                    break;
                }
                default:
                    // error log
                    break;
            }
        }
    }
}

