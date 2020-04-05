#version 330 core

out vec4 fragColor;
in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * lightColor;

    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(lightPos - vFragPos);

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0);
}
