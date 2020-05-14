#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;
layout (location = 4) in float aHeight;

out vec3 vNormal;
out vec2 vTextureCoord;
out vec3 vFragPos;
out float vHeight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vNormal = mat3(transpose(inverse(model))) * aNormal;
	vTextureCoord = aTextureCoord;
	vFragPos = vec3(model * vec4(aPos, 1.0));
    vHeight = aHeight;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
