#version 330 core

struct lightColor
{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture1;
uniform vec4 color;
uniform int imageEnable;
uniform lightColor light;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 pColor;

void main()
{
    if (imageEnable == 0) {
        // FragColor = color * vec4(light.diffuse, 1.0);
        FragColor = color + vec4(pColor, 1.0);
    } else {
        // FragColor = texture(texture1, TexCoord) * vec4(light.diffuse, 1.0);
        vec4 texCoord = texture(texture1, TexCoord);
        // if (texCoord.a < 0.001) discard;
        FragColor = texCoord;
    }
}
