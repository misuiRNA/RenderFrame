#include "ModelMeshLoader.h"
#include <iostream>


Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
: vertices(vertices)
, indices(indices)
, textures(textures) {

}

const std::vector<Mesh>& ModelMeshLoader::loadModelAsMeshes(std::string const& path) {
    _meshes.clear();
    _directory.clear();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return _meshes;
    }
    _directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    return _meshes;
}

void ModelMeshLoader::processNode(aiNode* node, const aiScene* scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh ModelMeshLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Mesh::Vertex> vertices;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Mesh::Vertex vertex;
        vertex.position = Vector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->HasNormals()) {
            vertex.normal = Vector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        if(mesh->mTextureCoords[0]) {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.texCoords = Vector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.tangent = Vector3D(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            vertex.bitangent = Vector3D(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        } else {
            vertex.texCoords = Vector2D(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);        
        }
    }

    std::vector<Mesh::Texture> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Mesh::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Mesh::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Mesh::Texture> ModelMeshLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType aiType) {
    Mesh::Texture::Type type = Mesh::Texture::Type::UNKNOWN;
    switch (aiType) {
        case aiTextureType_DIFFUSE:
            type = Mesh::Texture::Type::DIFFUSE;
            break;
        case aiTextureType_SPECULAR:
            type = Mesh::Texture::Type::SPECULAR;
            break;
        case aiTextureType_HEIGHT:
            type = Mesh::Texture::Type::NORMAL;
            break;
        case aiTextureType_AMBIENT:
            type = Mesh::Texture::Type::HEIGHT;
            break;
        default:
            break;
    }

    std::vector<Mesh::Texture> textures;
    for(unsigned int index = 0; index < mat->GetTextureCount(aiType); index++) {
        aiString aiTextureName;
        mat->GetTexture(aiType, index, &aiTextureName);
        std::string textureName = aiTextureName.C_Str();
        if (_texturesLoadedMap.find(textureName) == _texturesLoadedMap.end()) {
            LocalImage image(_directory + "/" + textureName);
            _texturesLoadedMap.emplace(textureName, Mesh::Texture(image, type));
        }
        textures.push_back(_texturesLoadedMap.at(textureName));
    }
    return textures;
}
