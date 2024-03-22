#version 330 core
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    gl_Position = cameraMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
}
