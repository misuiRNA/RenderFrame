#ifndef MODEL_OBJ_LOADER_H
#define MODEL_OBJ_LOADER_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

struct Model3DLoader {
    Model3DLoader(bool gammaCorrection = false) : _gammaCorrection(gammaCorrection) { }
    const std::vector<Mesh>& loadModel(std::string const& path);

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
