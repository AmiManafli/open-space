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

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
    vec3 color = vec3(texture(material.diffuse, vTextureCoord));
    vec3 normal = normalize(vNormal);

    // Ambient
    vec3 ambient = light.ambient * color;

    // Diffuse
    vec3 ligthDirection = normalize(light.position - vFragPos);
    vec3 diffuse = light.diffuse * max(dot(normal, ligthDirection), 0.0) * color;

    // Specular
    vec3 viewDirection = normalize(viewPos - vFragPos);
    vec3 reflectDirection = reflect(-ligthDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), 256);
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, vTextureCoord));

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
