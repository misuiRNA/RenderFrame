#include "ShaderMaterial.h"


ShaderMaterial::ShaderMaterial()
: diffuseTexture(0)
, specularTexture(0)
, ambient(0.0f, 0.0f, 0.0f)
, diffuse(0.0f, 0.0f, 0.0f)
, specular(0.0f, 0.0f, 0.0f)
, shininess(32.0f) {

}

ShaderMaterial::ShaderMaterial(const Color& ambient, const Color& diffuse, const Color& specular)
: diffuseTexture(0)
, specularTexture(0)
, ambient(ambient)
, diffuse(diffuse)
, specular(specular)
, shininess(32.0f) {

}

ShaderMaterial::ShaderMaterial(TextureId diffuseTexture, TextureId specularTextrue)
: diffuseTexture(diffuseTexture)
, specularTexture(specularTextrue)
, ambient(0.0f, 0.0f, 0.0f)
, diffuse(0.0f, 0.0f, 0.0f)
, specular(0.0f, 0.0f, 0.0f)
, shininess(32.0f) {

}
