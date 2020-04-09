#ifndef CG1_PROJECT_TERRAIN_H
#define CG1_PROJECT_TERRAIN_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>

#include <cg/terrain/Noise.h>


class Terrain : public MeshComponent {
public:
    ~Terrain() override;

    static Terrain* generate(uint32_t width, uint32_t height, ShaderProgram *shaderProgram, GLenum mode, Noise *noise);
    static void build(std::vector<MeshComponent::Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t width, uint32_t height, uint32_t subdivisionsWidth, uint32_t subdivisionsHeight);

    bool update(uint32_t width, uint32_t height, uint32_t subdivisionsWidth, uint32_t subdivisionsHeight, double maxTerrainHeight, double zoom);

private:
    Terrain(uint32_t width, uint32_t height, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures, ShaderProgram *shaderProgram, GLenum mode, Noise *noise);

    void updateHeights(double maxTerrainHeight, double zoom);
    glm::vec3 calculateNormal(int row, int col, double zoom);

    Noise *noise;
    uint32_t width, height, subdivisionsWidth, subdivisionsHeight;
};


#endif //CG1_PROJECT_TERRAIN_H
