#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include "BaseDefine.h"
#include "RenderData.h"


#define MAX_BONE_INFLUENCE 4

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
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
    : vertices(vertices)
    , indices(indices)
    , textures(textures) {

    }

public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
};

#endif