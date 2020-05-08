#version 330 core

in vec3 vPos;

out vec4 fragColor;

uniform vec3 viewPos;

/**
 * Calculate grid fade value using linear interpolation.
 */
float calcFade(float length, float maxDistance, float min, float max) {
    float k = (max - min) / (0 - maxDistance);

    return clamp(k * length + max, min, max);
}

void main() {
    float r, g, b;

    float maxDistance = 30.0;
    float length = sqrt(vPos.x * vPos.x + vPos.z * vPos.z);

    float fade = calcFade(length, maxDistance, 0.0, 0.2);
    float colorFade = calcFade(length, maxDistance, 0.0, 0.7);

    // X-axis
    if (vPos.y == 0 && vPos.z == 0) {
        r = colorFade;
    } else {
        r = fade;
    }
    // Y-axis
    if (vPos.x == 0 && vPos.z == 0) {
        g = colorFade;
    } else {
        g = fade;
    }
    // Z-axis
    if (vPos.x == 0 && vPos.y == 0) {
        b = colorFade;
    } else {
        b = fade;
    }
    fragColor = vec4(r, g, b, 1.0);
}
