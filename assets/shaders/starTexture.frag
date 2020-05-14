#version 330 core
layout (location = 0) out vec4 vFragColor;
layout (location = 1) out vec4 vBrightColor;

in vec3 vNormal;
in vec2 vTextureCoord;
in vec3 vFragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;
uniform vec3 viewPos;

vec3 calculateColor(vec3 materialDiffuse, vec3 materialSpecular) {
    return materialDiffuse + materialSpecular;
}

void main() {
    vec3 materialDiffuse = vec3(texture(material.diffuse, vTextureCoord));
    vec3 materialSpecular = vec3(texture(material.specular, vTextureCoord));

    vec3 result = calculateColor(materialDiffuse, materialSpecular);

    vFragColor = vec4(result, 1.0);

    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0) {
        vBrightColor = vec4(result, 1.0);
    } else {
        vBrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
