#version 330 core
in vec3 vNormal;
in vec2 vTextureCoord;
in vec3 vFragPos;

//layout (location = 0) out fragColor
//layout (location = 1) out brightColor (will be the "in vec2 aTextureCoord" in gaussianBlur.vert)

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

//    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
//    if (brightness > 1.0) {
//        brightColor = vec4(result, 1.0);
//    }
//    else {
//        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
//    }
}
