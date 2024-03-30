#version 330 core
struct Camera {
    vec3 pos;
    mat4 matrix;
};

uniform mat4 modelMatrix;
uniform Camera camera;

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = camera.matrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
