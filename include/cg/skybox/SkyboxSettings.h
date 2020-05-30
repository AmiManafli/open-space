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
    int seed;
    int resolution;
    int size;

    int numSmallPointStars;
    int numMediumPointStars;
    int numLargePointStars;

    int numBrightStars;

    bool enableNebulae;
    NebulaSettings nebulaSettings;

    bool saveToFile;
};

static SkyboxSettings DEFAULT_SKYBOX_SETTINGS = {
        1,
        3072,
        100000,

        92000,
        6000,
        2000,

        20,

        true,
        {
            glm::vec3(1, 0.05, 0),
            glm::vec3(0, 0, 0),
            1.486f,
            0.8f,
            3.311f,
        },

        false,
};

#endif //CG1_PROJECT_SKYBOXSETTINGS_H
