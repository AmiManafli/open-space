#ifndef CG1_PROJECT_MESHCOMPONENT_H
#define CG1_PROJECT_MESHCOMPONENT_H

#include <cg/GLHeader.h>
#include <cg/shaders/ShaderProgram.h>
#include <cg/entities/Component.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class MeshComponent : public Component {
public:
    enum {
        AttributePosition = 0,
        AttributeNormal = 1,
        AttributeTextureCoord = 2,
        AttributeInstances = 3,
        AttributeHeight = 4,
    };

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoord;
        float height;
    };

    struct Texture {
        uint32_t id;
        std::string type;
		std::string path;
		bool isCubeMap;
    };

    struct Material {
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };

public:
    MeshComponent() {}
    MeshComponent(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures, ShaderProgram *shaderProgram, GLenum mode);
    ~MeshComponent() override;

    static std::vector<MeshComponent *> createMeshComponentsFromFile(std::string filename, ShaderProgram *shaderProgram);

    void createInstances(std::vector<glm::vec3>& transformations);

    virtual void subdivide(uint16_t subdivisions) {}
    virtual void setColor(glm::vec3 color) {}

    uint32_t vao, vbo, instanceVbo, ebo;
    ShaderProgram *shaderProgram;
    GLenum mode;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;
    uint32_t instances;
    bool indexed;
    std::vector<Material> materials;

    uint16_t subdivision;
    glm::vec3 color;

    float maxHeight;

    void setupBuffers();

protected:
	static std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	static uint32_t textureFromFile(const char* path, const std::string& directory);
	
	static void processNode(std::vector<MeshComponent *>& meshes, aiNode *node, const aiScene *scene, ShaderProgram *shaderProgram);
    static MeshComponent* processMesh(std::vector<MeshComponent *>& meshes, aiMesh *mesh, const aiScene *scene, ShaderProgram *shaderProgram);
};

#endif //CG1_PROJECT_MESHCOMPONENT_H
