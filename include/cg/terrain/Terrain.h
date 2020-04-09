#ifndef CG1_PROJECT_TERRAIN_H
#define CG1_PROJECT_TERRAIN_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>


class OpenSimplexNoise;

class Terrain : public MeshComponent {
public:
    ~Terrain() override;

    static Terrain* generate(uint32_t width, uint32_t height, ShaderProgram *shaderProgram, GLenum mode);
    static void build(std::vector<MeshComponent::Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t width, uint32_t height, uint32_t subdivisionsWidth, uint32_t subdivisionsHeight);

    bool update(uint32_t width, uint32_t height, uint32_t subdivisionsWidth, uint32_t subdivisionsHeight, double maxTerrainHeight, double zoom);

private:
    Terrain(uint32_t width, uint32_t height, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures, ShaderProgram *shaderProgram, GLenum mode);

    void updateHeights(OpenSimplexNoise& noise, double maxTerrainHeight, double zoom);
    glm::vec3 calculateNormal(OpenSimplexNoise& noise, int row, int col, double zoom);

    uint32_t width, height, subdivisionsWidth, subdivisionsHeight;
};


#endif //CG1_PROJECT_TERRAIN_H
