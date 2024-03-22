#version 330 core
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = cameraMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
