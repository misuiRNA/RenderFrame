#version 330 core
uniform sampler2D texture1;
uniform vec4 color;
uniform int imageEnable;
uniform vec3 lightColor;

out vec4 FragColor;

in vec2 TexCoord;

void main()
{
    if (imageEnable == 0) {
        FragColor = color * vec4(lightColor, 1.0);
    } else {
        FragColor = texture(texture1, TexCoord) * vec4(lightColor, 1.0);
    }
}
