#ifndef CG1_PROJECT_ENTITYBUILDER_H
#define CG1_PROJECT_ENTITYBUILDER_H

#include "EntityManager.h"
#include <cg/entities/components/CameraComponent.h>
#include <cg/entities/components/LightComponent.h>
#include <cg/entities/components/MassComponent.h>
#include <cg/entities/components/SelectableComponent.h>
#include <cg/entities/components/CollisionComponent.h>


class EntityBuilder {
public:
    static EntityBuilder* create() { return new EntityBuilder(); }

    Entity* build(EntityManager *entityManager);
    void destroy();

    EntityBuilder* withTransform(TransformComponent *transform) { transformComponent = transform; return this; }
    EntityBuilder* withTransform(float x, float y, float z);
    EntityBuilder* withTransform(glm::vec3 position);
    EntityBuilder* withScale(float scale);

    EntityBuilder* withMesh(MeshComponent *mesh) { meshComponents.push_back(mesh); return this; }
    EntityBuilder* withMesh(std::vector<MeshComponent *> meshes) { meshComponents = meshes; return this; }
    EntityBuilder* withMesh(std::string filename, ShaderProgram *shaderProgram);
    EntityBuilder* withMesh(std::vector<MeshComponent::Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<MeshComponent::Texture>& textures, ShaderProgram *shaderProgram, GLenum mode);
    EntityBuilder* withInstances(std::vector<glm::vec3>& transformations);

    EntityBuilder* withCamera(CameraComponent *camera) { cameraComponent = camera; return this; }
    EntityBuilder* withCamera(CameraComponent::Mode mode, CameraComponent::Type type, glm::vec3 target, glm::vec3 front, glm::vec3 up, float aspectRatio);

    EntityBuilder* withHighlight(HighlightComponent *highlight) { highlightComponent = highlight; return this; }
    EntityBuilder* withHighlight(float scaleFactor, ShaderProgram *shaderProgram);

    EntityBuilder* withVelocity(VelocityComponent *velocity) { velocityComponent = velocity; return this; }
    EntityBuilder* withVelocity(glm::vec3 velocity, std::function<void(EntityManager *, Entity *)> customUpdate = nullptr);

    EntityBuilder* withLight(LightComponent *light);
    EntityBuilder* withDirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    EntityBuilder* withPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);

    EntityBuilder* withMass(double mass);
    EntityBuilder* withMass(MassComponent *component);

    EntityBuilder* withOrbit(TransformComponent *parentTransform, double semiMajorAxis, double semiMinorAxis, double speed, double startTheta);
    EntityBuilder* withOrbit(OrbitComponent *component);

    EntityBuilder* withSphereCollision(float radius);

    EntityBuilder* isSelectable();

private:
    std::vector<MeshComponent *> meshComponents;
    TransformComponent *transformComponent = nullptr;
    CameraComponent *cameraComponent = nullptr;
    HighlightComponent *highlightComponent = nullptr;
    VelocityComponent *velocityComponent = nullptr;
    LightComponent *lightComponent = nullptr;
    MassComponent *massComponent = nullptr;
    OrbitComponent *orbitComponent = nullptr;
    SelectableComponent *selectableComponent = nullptr;
    CollisionComponent *collisionComponent = nullptr;
};

#endif //CG1_PROJECT_ENTITYBUILDER_H
