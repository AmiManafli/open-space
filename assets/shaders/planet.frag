#version 410 core
in vec3 vNormal;
in vec2 vTextureCoord;
in vec3 vFragPos;
in float vHeight;

out vec4 fragColor;

const int MAX_LIGHTS = 2;
const int LIGHT_TYPE_DIRECTIONAL = 0;
const int LIGHT_TYPE_POINT = 1;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct MaterialColor {
    vec3 diffuse;
    vec3 specular;
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
uniform MaterialColor materialColor[3];
uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;
uniform float maxHeight;

vec3 calculateDirectionalLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular, float power) {
    vec3 lightDirection = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.ambient * materialDiffuse;

    // Diffuse
    vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * materialDiffuse;

    // Specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), power) * 0.3;
    vec3 specular = light.specular * specularValue * materialSpecular;

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular, float power) {
    float distance = length(light.position - vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 lightDirection = normalize(light.position - vFragPos);

    // Ambient
    vec3 ambient = light.ambient * materialDiffuse;

    // Diffuse
    vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * materialDiffuse;

    // Specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), power) * 0.3;
    vec3 specular = light.specular * specularValue * materialSpecular;

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 normal = normalize(vNormal);

    vec3 viewDirection = normalize(viewPos - vFragPos);

    vec3 waterColor = materialColor[0].diffuse;
    vec3 grassColor = materialColor[1].diffuse;
    vec3 peakColor = materialColor[2].diffuse;

    vec3 waterSpecular = materialColor[0].specular;
    vec3 grassSpecular = materialColor[1].specular;

    float shoreHeight = maxHeight * 0.1;
    float lowPeakHeight = maxHeight * 0.6;
    float peakHeight = maxHeight * 0.9;

    vec3 diffuse;
    vec3 specular = vec3(0.1, 0.1, 0.1);
    float power;
    if (vHeight <= shoreHeight) {
        float value = clamp(vHeight, 0, shoreHeight) / shoreHeight;
        specular = mix(waterSpecular, grassSpecular, value);
        diffuse = mix(waterColor, grassColor, value);
        power = 2;
    } else if (vHeight < lowPeakHeight) {
        diffuse = grassColor;
        power = 2;
    } else {
        float value = clamp(vHeight - lowPeakHeight, 0, maxHeight - lowPeakHeight) / (maxHeight - lowPeakHeight);
        diffuse = mix(grassColor, peakColor, value);
        power = 1;
    }

    vec3 materialDiffuse = diffuse;
    vec3 materialSpecular = specular;

    vec3 color;
    for (int i = 0; i < lights.length(); i++) {
        Light light = lights[i];

        vec3 result;
        if (light.type == LIGHT_TYPE_DIRECTIONAL) {
            result = calculateDirectionalLight(light, normal, viewDirection, materialDiffuse, materialSpecular, power);
        } else {
            result = calculatePointLight(light, normal, viewDirection, materialDiffuse, materialSpecular, power);
        }
        color += result;
    }

    fragColor = vec4(color, 1.0);
}
