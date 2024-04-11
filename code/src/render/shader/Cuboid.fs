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

struct lightColor
{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 color;
uniform int imageEnable;
uniform Material material;
uniform lightColor light;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 CameraPos;

out vec4 FragColor;

vec3 calcAmbient(vec3 materialAmbient)
{
    vec3 ambient = light.ambient * materialAmbient;
    return ambient;
}

vec3 calcDiffuse(vec3 materialDiffuse)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * materialDiffuse);
    return diffuse;
}

vec3 calcSpecular(vec3 materialSpecular)
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(CameraPos - FragPos);
    vec3 lightDir = normalize(light.pos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * materialSpecular);
    return specular;
}

void main()
{
    vec3 ambient = calcAmbient(vec3(texture(material.diffuseTexture, TexCoord)) + material.ambient);
    vec3 diffuse = calcDiffuse(vec3(texture(material.diffuseTexture, TexCoord)) + material.diffuse);
    vec3 specular = calcSpecular(vec3(texture(material.specularTexture, TexCoord)) + material.specular);

    vec4 gouraudLight = vec4(ambient + diffuse + specular, 1.0);

    if (imageEnable == 0)
    {
        FragColor = gouraudLight * color;
    }
    else
    {
        FragColor = gouraudLight * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    }
}
