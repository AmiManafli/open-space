#ifndef CG1_PROJECT_ENTITYBUILDER_H
#define CG1_PROJECT_ENTITYBUILDER_H

#include <entities/components/CameraComponent.h>
#include "EntityManager.h"

class EntityBuilder {
public:
    explicit EntityBuilder(EntityManager *entityManager);
    ~EntityBuilder();

    EntityBuilder* withPosition(float x, float y, float z);
    EntityBuilder* withPosition(glm::vec3 position);

    EntityBuilder* withMesh(std::string filename, ShaderProgram *shaderProgram);
    EntityBuilder* withMesh(std::vector<MeshComponent::Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<MeshComponent::Texture>& textures, ShaderProgram *shaderProgram);

    EntityBuilder* withCamera(CameraComponent::Mode mode, CameraComponent::Type type, glm::vec3 target, glm::vec3 front, glm::vec3 up, float aspectRatio);

    EntityBuilder* withHighlight(float scaleFactor, ShaderProgram *shaderProgram);

    Entity* build();
    void destroy();

private:
    EntityManager *entityManager;

    std::vector<MeshComponent *> meshComponents;
    PositionComponent *positionComponent = nullptr;
    CameraComponent *cameraComponent = nullptr;
    HighlightComponent *highlightComponent = nullptr;
};

#endif //CG1_PROJECT_ENTITYBUILDER_H
