#version 330 core
in vec3 vNormal;
in vec2 vTextureCoord;
in vec3 vFragPos;

out vec4 fragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirectionalLight directionalLight;
uniform vec3 viewPos;

void main() {
    vec3 color = vec3(texture(material.diffuse, vTextureCoord));
    vec3 normal = normalize(vNormal);

    vec3 lightDirection = normalize(-directionalLight.direction);

    // Ambient
    vec3 ambient = directionalLight.ambient * color;

    // Diffuse
    vec3 diffuse = directionalLight.diffuse * max(dot(normal, lightDirection), 0.0) * color;

    // Specular
    vec3 viewDirection = normalize(viewPos - vFragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
    vec3 specular = directionalLight.specular * specularValue * vec3(texture(material.specular, vTextureCoord));

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
