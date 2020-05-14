#version 330 core

out vec4 fragColor;

in vec2 vTextureCoord;

uniform sampler2D scene;

uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, vTextureCoord).rgb;
    vec3 bloomColor = texture(bloomBlur, vTextureCoord).rgb;
    if (bloom) {
        hdrColor += bloomColor;
    }
//    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
//    result = pow(result, vec3(1.0 / gamma));
    vec3 result = hdrColor;
    fragColor = vec4(result, 1.0);
}