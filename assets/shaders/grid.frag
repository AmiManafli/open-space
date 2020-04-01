#version 330 core

in vec3 vPos;

out vec4 fragColor;

void main() {
    float length = sqrt(vPos.x * vPos.x + vPos.z * vPos.z);
    float fade = clamp(-0.01 * length + 0.3, 0.2, 0.3);
    float r, g, b;

    if (vPos.x == 0) {
        r = 1.0;
    } else {
        r = fade;
    }
    if (vPos.z == 0) {
        b = 1.0;
    } else {
        b = fade;
    }
    g = fade;
    fragColor = vec4(r, g, b, 1.0);
}
