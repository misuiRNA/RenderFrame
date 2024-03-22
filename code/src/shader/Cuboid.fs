#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 color;
uniform int imageEnable;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float specularStrength;
uniform int shininess;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

vec3 calcAmbient()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    return ambient;
}

vec3 calcDiffuse()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    return diffuse;
}

vec3 calcSpecular()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    return specular;
}

void main()
{
    vec3 ambient = calcAmbient();
    vec3 diffuse = calcDiffuse();
    vec3 specular = calcSpecular();

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
