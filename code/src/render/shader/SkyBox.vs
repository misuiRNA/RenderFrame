#version 330 core

struct Camera
{
    vec3 pos;
    mat4 matrix;
};

uniform Camera camera;
uniform mat4 modelMatrix;

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

void main()
{
    TexCoords = aPos;
    vec4 pos = camera.matrix * modelMatrix * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
