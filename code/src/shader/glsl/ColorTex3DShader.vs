#version 330 core
struct Camera
{
    vec3 pos;
    mat4 matrix;
};

uniform Camera camera;
uniform mat4 modelMatrix;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;

out vec2 TexCoord;
out vec3 pColor;

void main()
{
    gl_Position = camera.matrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
    pColor = aColor;
}
