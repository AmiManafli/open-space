#version 330 core

out vec4 fragColor;
in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    float ambientStrenght = 0.3;
    vec3 ambient = ambientStrenght * lightColor;

    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(lightPos - vFragPos);

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}
