#include "object/Model.h"
#include "ModelMeshLoader.h"
#include "Utils.h"


Model3DDrawObject::Model3DDrawObject(std::string const& path)
: _pos({0.0f, 0.0f, 0.0f}) {
    load(path);
}

void Model3DDrawObject::setPosition(const Position& pos) {
    _pos = pos;
    for (auto& draw : _meshDrawes) {
        draw.setPosition(pos);
    }
}

void Model3DDrawObject::move(const Position& offset) {
    setPosition(_pos + offset);
}

void Model3DDrawObject::setSize(const Size3D& size) {
    float ratioX = size.x / _modelBox.lenX();
    float ratioY = size.y / _modelBox.lenY();
    float ratioZ = size.z / _modelBox.lenZ();
    float ratioTemp = 0.0f;
    ratioTemp = std::max(ratioX, ratioTemp);
    ratioTemp = std::max(ratioY, ratioTemp);
    ratioTemp = std::max(ratioZ, ratioTemp);
    if (ratioTemp <= 1e-6) {
        printf("Model3DDrawObject::setSize: size is zero\n");
        return;
    }
    ratioX = ((ratioX > 1e-6) ? ratioX : ratioTemp);
    ratioY = ((ratioY > 1e-6) ? ratioY : ratioTemp);
    ratioZ = ((ratioZ > 1e-6) ? ratioZ : ratioTemp);
    Size3D scale = Size3D(ratioX, ratioY, ratioZ);
    for (auto& draw : _meshDrawes) {
        draw.setSize(scale);
    }
}

void Model3DDrawObject::setAttituedeCtrl(const Vector3D& up, const Vector3D& front) {
    for (auto& draw : _meshDrawes) {
        draw.getTransform()
           .setUp(up)
           .setFront(front);
    }
}

void Model3DDrawObject::show() {
    for (auto& draw : _meshDrawes) {
        draw.show();
    }
}

Position Model3DDrawObject::getPosition() {
    return _pos;
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

        for (const Mesh::Vertex& vert : mesh.vertices) {
            _modelBox.right = std::max(_modelBox.right, vert.position.x);
            _modelBox.left = std::min(_modelBox.right, vert.position.x);
            _modelBox.top = std::max(_modelBox.top, vert.position.y);
            _modelBox.bottom = std::min(_modelBox.bottom, vert.position.y);
            _modelBox.front = std::max(_modelBox.front, vert.position.z);
            _modelBox.back = std::min(_modelBox.back, vert.position.z);
        }
    }

    setSize({1.0f});
}

