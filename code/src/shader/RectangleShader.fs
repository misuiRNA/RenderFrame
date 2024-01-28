#version 330 core
uniform sampler2D texture1;
uniform vec4 color;
uniform int imageEnable;

out vec4 FragColor;

in vec2 TexCoord;

void main()
{
    if (imageEnable == 0) {
        FragColor = color;
    } else {
        FragColor = texture(texture1, TexCoord);
    }
}
