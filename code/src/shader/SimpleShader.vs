#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 projection;

out vec3 pColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    pColor = aColor;
    TexCoord = aTexCoord;
}
