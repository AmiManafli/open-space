#ifndef CG1_PROJECT_TERRAIN_H
#define CG1_PROJECT_TERRAIN_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>

#include <cg/terrain/Noise.h>

enum NoiseType {
    OpenSimplex,
    Perlin,
};

struct TerrainSettings {
    int width;
    int height;
    int subdivisionWidth;
    int subdivisionHeight;
    double maxHeight;
    double frequency;
    int octaves;
    int seed;
    NoiseType noiseType;
};


class Terrain : public MeshComponent {
public:
    ~Terrain() override;

    static Terrain* generate(uint32_t width, uint32_t height, ShaderProgram *shaderProgram, GLenum mode, NoiseType noiseType);
    static void build(std::vector<MeshComponent::Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t width, uint32_t height, uint32_t subdivisionsWidth, uint32_t subdivisionsHeight);

    bool update(TerrainSettings& settings);

    Noise* getNoise();

private:
    Terrain(uint32_t width, uint32_t height, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures, ShaderProgram *shaderProgram, GLenum mode, NoiseType noiseType);

    void updateHeights(TerrainSettings& settings);
    void updateNormals(TerrainSettings& settings);

    NoiseType noiseType;
    uint32_t width, height, subdivisionsWidth, subdivisionsHeight;

    Noise *openSimplexNoise;
    Noise *perlinNoise;
};


#endif //CG1_PROJECT_TERRAIN_H
