#version 330 core
in vec3 vNormal;

out vec4 fragColor;

void main() {
    vec3 color = vec3(0.6, 0.6, 0.6);
    fragColor = vec4(color, 1.0);
}
