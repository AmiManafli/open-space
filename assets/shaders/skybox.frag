#version 330 core
in vec3 vNormal;
in vec3 vTexCoords;

out vec4 fragColor;

uniform samplerCube skybox;

void main() {
//    vec3 color = 0.5 * normalize(vNormal) + 0.5;
//    fragColor = vec4(color, 1.0);
    fragColor = texture(skybox, vTexCoords);
}
