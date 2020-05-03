#version 330 core
in vec3 vNormal;

out vec4 fragColor;

void main() {
//    vec3 color = 0.5 * normalize(vNormal) + 0.5;
    vec3 color = vec3(1, 1, 1);
    fragColor = vec4(color, 1.0);
}
