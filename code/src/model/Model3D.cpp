#include "model/Model3D.h"
#include "ModelMeshLoader.h"
#include "Utils.h"

struct Model3DVertex {
    Model3DVertex(const Vector3D& pos, const Vector3D& normal, const Vector2D& texCoords)
    : pos(pos)
    , normal(normal)
    , texCoords(texCoords) {

    }

    Vector3D pos;
    Vector3D normal;
    Vector2D texCoords;
};

static const std::vector<ShaderAttribDescriptor> MODEL3DVERTEX_DESCRIPTOR = {
    DESC("aPos",       0, Model3DVertex, pos),
    DESC("aNormal",    1, Model3DVertex, normal),
    DESC("aTexCoords", 2, Model3DVertex, texCoords),
};

static ShaderProgram& GetShaderProg() {
    static const std::string VS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Model3DlShader.vs");
    static const std::string FS_SHADER_STR = ReadFile(GetCurPath() + "/code/src/render/shader/Model3DlShader.fs");
    static ShaderProgram prog(VS_SHADER_STR, FS_SHADER_STR);
    return prog;
}


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
: AbstractDrawObject(GetShaderProg(), RenderDataMode::TRIANGLES)
, _attitudeCtrl({0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f})
, _modelPath(path) {

}

void Model3D::setPosition(const Position& pos) {
    _attitudeCtrl.setPosition(pos);
}

void Model3D::setSize(const Size3D& size) {
    _attitudeCtrl.setSize(size);
}

Attitude3DController& Model3D::getAttituedeCtrl() {
    return _attitudeCtrl;
}

void Model3D::updateUniformes() {
    _renderData.setUniform("model", _attitudeCtrl.getMatrix());
}

void Model3D::updateRenderData() {
    ModelMeshLoader loader;
    const std::vector<Mesh>& meshes = loader.loadModelAsMeshes(_modelPath);
    std::vector<RenderData> renderDatas;
    renderDatas.reserve(meshes.size());
    for (const Mesh& mesh : meshes) {
        RenderData data(_renderData.getShaderProgram(), RenderDataMode::TRIANGLES);
        data.setVertices(MeshVertex2Vertex(mesh.vertices), MODEL3DVERTEX_DESCRIPTOR);
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
