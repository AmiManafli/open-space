#ifndef CG1_PROJECT_TERRAIN_H
#define CG1_PROJECT_TERRAIN_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>


class Terrain : public MeshComponent {
public:
    ~Terrain() override;

    static Terrain* generate(uint32_t width, uint32_t height, ShaderProgram *shaderProgram, GLenum mode);
    static void build(std::vector<MeshComponent::Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t width, uint32_t height, uint32_t subdivisionLevel);

    void subdivide(uint32_t level);

private:
    Terrain(uint32_t width, uint32_t height, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures, uint32_t subdivisionLevel, ShaderProgram *shaderProgram, GLenum mode);

    uint32_t width, height, subdivisionLevel;
};


#endif //CG1_PROJECT_TERRAIN_H
