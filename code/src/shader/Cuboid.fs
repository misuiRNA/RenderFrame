#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 color;
uniform int imageEnable;
uniform vec4 lightColor;

void main()
{
    if (imageEnable == 0)
    {
        FragColor = color * lightColor;
    }
    else
    {
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * lightColor;
    }
}
