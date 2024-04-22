#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuseTexture[2];

void main()
{
    FragColor = texture(diffuseTexture[0], TexCoords);
}
