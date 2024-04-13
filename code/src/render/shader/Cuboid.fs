#version 330 core

struct Material
{
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct ParallelLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Light
{
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float attenuationKC;
    float attenuationKL;
    float attenuationKQ;

    vec3  spotDirection;
    float spotCos;
    float spotOuterCos;
};

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 color;
uniform int imageEnable;
uniform Material material;
uniform Light light;
uniform ParallelLight parallelLight;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 CameraPos;

out vec4 FragColor;

float calcSpotIntensity(Light pLight)
{
    // 点光源才会聚光
    vec3 lightDir = normalize(FragPos - pLight.pos);
    float theta = dot(-lightDir, normalize(-pLight.spotDirection));
    float epsilon  = pLight.spotCos - pLight.spotOuterCos;
    float intensity = clamp((theta - pLight.spotOuterCos) / epsilon, 0.0, 1.0);
    return intensity;
}

float calcAttenuation(Light pLight)
{
    // 点光源才会衰减
    float distance = length(pLight.pos - FragPos);
    float attenuation = 1.0 / (pLight.attenuationKC + pLight.attenuationKL * distance +  pLight.attenuationKQ * (distance * distance));
    return attenuation;
}


vec3 calcAmbient(vec3 materialAmbient, vec3 pLightAmbient)
{
    vec3 ambient = pLightAmbient * materialAmbient;
    return ambient;
}

vec3 calcDiffuse(vec3 materialDiffuse, vec3 pLightDir, vec3 pLightDiffuse)
{
    vec3 lightDir = normalize(-pLightDir);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pLightDiffuse * (diff * materialDiffuse);
    return diffuse;
}

vec3 calcSpecular(vec3 materialSpecular, vec3 pLightDir, vec3 pLightSpecular)
{
    vec3 lightDir = normalize(-pLightDir);
    vec3 viewDir = normalize(CameraPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pLightSpecular * (spec * materialSpecular);
    return specular;
}

// 计算点光源的光照
vec4 pointLight_getGouraudLight(Light pLight, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular)
{
    vec3 pointLightDir = normalize(FragPos - pLight.pos);
    vec3 ambient = calcAmbient(materialAmbient, pLight.ambient);
    vec3 diffuse = calcDiffuse(materialDiffuse, pointLightDir, pLight.diffuse);
    vec3 specular = calcSpecular(materialSpecular, pointLightDir, pLight.specular);

    float attenuation = calcAttenuation(pLight);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float intensity = calcSpotIntensity(pLight);
    diffuse  *= intensity;
    specular *= intensity;

    vec4 gouraudLight = vec4(ambient + diffuse + specular, 1.0);
    return gouraudLight;
}

// 计算平行光源的光照
vec4 parallelLight_getGouraudLight(ParallelLight pLight, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular)
{
    vec3 paralletLightDir = normalize(parallelLight.direction);
    vec3 ambient = calcAmbient(materialAmbient, parallelLight.ambient);
    vec3 diffuse = calcDiffuse(materialDiffuse, paralletLightDir, parallelLight.diffuse);
    vec3 specular = calcSpecular(materialSpecular, paralletLightDir, parallelLight.specular);

    vec4 gouraudLight = vec4(ambient + diffuse + specular, 1.0);
    return gouraudLight;
}


void main()
{
    vec3 materialAmbient = vec3(texture(material.diffuseTexture, TexCoord)) + material.ambient;
    vec3 materialDiffuse = vec3(texture(material.diffuseTexture, TexCoord)) + material.diffuse;
    vec3 materialSpecular = vec3(texture(material.specularTexture, TexCoord)) + material.specular;

    vec4 gouraudLight = pointLight_getGouraudLight(light, materialAmbient, materialDiffuse, materialSpecular);
    // vec4 gouraudLight = parallelLight_getGouraudLight(parallelLight, materialAmbient, materialDiffuse, materialSpecular);

    if (imageEnable == 0)
    {
        FragColor = gouraudLight * color;
    }
    else
    {
        FragColor = gouraudLight * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3);
    }
}
