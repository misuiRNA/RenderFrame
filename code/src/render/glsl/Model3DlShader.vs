#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

struct Camera
{
    vec3 pos;
    mat4 matrix;
};

uniform Camera camera;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = camera.matrix * model * vec4(aPos, 1.0);
}
