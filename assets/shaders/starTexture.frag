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

uniform Material material;
uniform vec3 viewPos;

vec3 calculateColor(vec3 materialDiffuse, vec3 materialSpecular) {
    return materialDiffuse + materialSpecular;
}

void main() {
    vec3 materialDiffuse = vec3(texture(material.diffuse, vTextureCoord));
    vec3 materialSpecular = vec3(texture(material.specular, vTextureCoord));

    vec3 result = calculateColor(materialDiffuse, materialSpecular);

    fragColor = vec4(result, 1.0);
}
