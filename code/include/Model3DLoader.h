#ifndef MODEL_OBJ_LOADER_H
#define MODEL_OBJ_LOADER_H

#include "BaseDefine.h"
#include "RenderData.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include <vector>


struct Texture {
    enum class Type {
        DIFFUSE  = 0,
        SPECULAR = 1,
        NORMAL   = 2,
        HEIGHT   = 3,
    };

    Type type;
    unsigned int id;
    std::string key;
};

struct Mesh {
    static constexpr int MAX_BONE_INFLUENCE = 4;

    struct Vertex {
        Vector3D position;
        Vector3D normal;
        Vector2D texCoords;
        Vector3D tangent;
        Vector3D bitangent;
        //bone indexes which will influence this vertex
        int boneIds[MAX_BONE_INFLUENCE];
        //weights from each bone
        float weights[MAX_BONE_INFLUENCE];

        static std::vector<ShaderAttribDescriptor> descriptor;
    };

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
};


struct Model3DLoader {
    Model3DLoader(bool gammaCorrection = false) : _gammaCorrection(gammaCorrection) { }
    const std::vector<Mesh>& loadModelAsMeshes(std::string const& path);

private:
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType aiType, Texture::Type type);

private:
    bool _gammaCorrection;
    std::vector<Mesh> _meshes;
    std::string _directory;
    std::map<std::string, Texture> _texturesLoadedMap;
};


#endif
