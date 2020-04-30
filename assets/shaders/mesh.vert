#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

flat out vec3 vNormal;
out vec3 vFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vNormal = aNormal;
    vFragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
