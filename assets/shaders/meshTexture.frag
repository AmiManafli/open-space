#version 410 core
in vec3 vNormal;
in vec2 vTextureCoord;
in vec3 vFragPos;

out vec4 fragColor;

const int MAX_LIGHTS = 2;
const int LIGHT_TYPE_DIRECTIONAL = 0;
const int LIGHT_TYPE_POINT = 1;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    int type;

    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;

vec3 calculateDirectionalLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {
    vec3 lightDirection = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.ambient * materialDiffuse;

    // Diffuse
    vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * materialDiffuse;

    // Specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
    vec3 specular = light.specular * specularValue * materialSpecular;

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {
    float distance = length(light.position - vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 lightDirection = normalize(light.position - vFragPos);

    // Ambient
    vec3 ambient = light.ambient * materialDiffuse;

    // Diffuse
    vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * materialDiffuse;

    // Specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
    vec3 specular = light.specular * specularValue * materialSpecular;

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 normal = normalize(vNormal);

    vec3 viewDirection = normalize(viewPos - vFragPos);

    vec3 materialDiffuse = vec3(texture(material.diffuse, vTextureCoord));
    vec3 materialSpecular = vec3(texture(material.specular, vTextureCoord));

    vec3 color;
    for (int i = 0; i < lights.length(); i++) {
        Light light = lights[i];

        vec3 result;
        if (light.type == LIGHT_TYPE_DIRECTIONAL) {
            result = calculateDirectionalLight(light, normal, viewDirection, materialDiffuse, materialSpecular);
        } else {
            result = calculatePointLight(light, normal, viewDirection, materialDiffuse, materialSpecular);
        }
        color += result;
    }

    fragColor = vec4(color, 1.0);
}
