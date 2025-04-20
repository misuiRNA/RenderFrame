#ifndef _HEAD_FLAG_SHADERMATERIAL_H
#define _HEAD_FLAG_SHADERMATERIAL_H

#include "base/BaseDefine.h"
#include "Texture.h"

struct ShaderMaterial {
    ShaderMaterial();
    ShaderMaterial(const Color& ambient, const Color& diffuse, const Color& specular);
    ShaderMaterial(TextureId diffuseTexture, TextureId specularTextrue);

    const TextureId diffuseTexture;
    const TextureId specularTexture;
    const Color ambient;
    const Color diffuse;
    const Color specular;
    const float shininess;
};


#endif // _HEAD_FLAG_SHADERMATERIAL_H
