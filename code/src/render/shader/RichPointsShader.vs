#version 330 core

uniform mat4 modelMatrix;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out VERTEX_ATTRIBUT {
    vec3 color;
} vs_out;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    vs_out.color = aColor;
}
