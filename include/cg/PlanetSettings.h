#ifndef CG1_PROJECT_PLANETSETTINGS_H
#define CG1_PROJECT_PLANETSETTINGS_H

enum FaceDirection {
    FrontFace,
    BackFace,
    RightFace,
    LeftFace,
    UpFace,
    DownFace,
};

enum PlanetNoiseType {
    Simple,
    Ridged,
};

struct PlanetNoiseSettings {
    PlanetNoiseType noiseType;

    bool enabled;
    bool useFirstLayerAsMask;

    float strength;
    float baseRoughness;
    float roughness;
    float persistence;
    float minValue;
    float weightMultiplier;

    int layers;
    glm::vec3 center;
};

struct PlanetSettings {
    float radius;
    int subdivision;
    int seed;
    FaceDirection direction;
    std::vector<PlanetNoiseSettings> noiseSettings;
};

#endif //CG1_PROJECT_PLANETSETTINGS_H
