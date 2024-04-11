#ifndef _HEAD_FLAG_SHADERMATERIAL_H
#define _HEAD_FLAG_SHADERMATERIAL_H

#include "BaseDefine.h"

struct ShaderMaterial {
    ShaderMaterial();
    ShaderMaterial(const Color& ambient, const Color& diffuse, const Color& specular);
    ShaderMaterial(int diffuseTexture, int specularTextrue);

    const unsigned int diffuseTexture;
    const unsigned int specularTexture;
    const Color ambient;
    const Color diffuse;
    const Color specular;
    const float shininess;
};


#endif // _HEAD_FLAG_SHADERMATERIAL_H
