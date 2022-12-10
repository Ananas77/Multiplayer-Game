#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat3 normalTransform;

out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vFragPos;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    vFragPos = vec3(model * vec4(aPos, 1.0f));
    vNormal = normalize(aNormal * normalTransform);
    vTexCoord = aTexCoord;
}