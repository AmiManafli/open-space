#ifndef CG1_PROJECT_SKYBOXSETTINGS_H
#define CG1_PROJECT_SKYBOXSETTINGS_H

struct NebulaSettings {
    glm::vec3 color;
    glm::vec3 offset;
    float scale;
    float intensity;
    float falloff;
};

struct SkyboxSettings {
    int resolution;
    int size;

    int numPointStars;
    float smallPointStars;
    float mediumPointStars;
    float largePointStars;

    int numBrightStars;

    bool enableNebulae;
    NebulaSettings nebulaSettings;

    bool saveToFile;
};

static SkyboxSettings DEFAULT_SKYBOX_SETTINGS = {
        3072,
        5000,

        100000,
        0.92,
        0.06,
        0.02,

        20,

        true,
        {
            glm::vec3(1, 0, 0),
            glm::vec3(0, 0, 0),
            0.25f,
            0.9f,
            3.0f,
        },

        false,
};

#endif //CG1_PROJECT_SKYBOXSETTINGS_H
