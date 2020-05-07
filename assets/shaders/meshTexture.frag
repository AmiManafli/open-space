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

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {
    vec3 lightDirection = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.ambient * materialDiffuse;

    // Diffuse
    vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * materialDiffuse;

    // Specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
    vec3 specular = light.specular * specularValue * materialSpecular;

    return ambient + diffuse;// + specular;
}

void main() {
    vec3 normal = normalize(vNormal);

    vec3 viewDirection = normalize(viewPos - vFragPos);

    vec3 materialDiffuse = vec3(texture(material.diffuse, vTextureCoord));
    vec3 materialSpecular = vec3(texture(material.specular, vTextureCoord));

    vec3 result = calculateDirectionalLight(directionalLight, normal, viewDirection, materialDiffuse, materialSpecular);

    fragColor = vec4(result, 1.0);
}
