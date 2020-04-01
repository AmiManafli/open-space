#version 330 core

in vec3 vPos;

out vec4 fragColor;

/**
 * Calculate grid fade value using linear interpolation.
 */
float calcFade(float length, float maxDistance, float min, float max) {
    float k = (max - min) / (0 - maxDistance);

    return clamp(k * length + max, min, max);
}

void main() {
    float r, g, b;

    float maxDistance = 10.0;
    float length = sqrt(vPos.x * vPos.x + vPos.z * vPos.z);

    float fade = calcFade(length, maxDistance, 0.2, 0.3);
    float colorFade = calcFade(length, maxDistance, 0.2, 1.0);

    if (vPos.x == 0) {
        r = colorFade;
    } else {
        r = fade;
    }
    if (vPos.z == 0) {
        b = colorFade;
    } else {
        b = fade;
    }
    g = fade;
    fragColor = vec4(r, g, b, 1.0);
}
