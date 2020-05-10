#include <cg/collision/BoundingSphere.h>
#include <cg/entities/components/CollisionComponent.h>
#include "cg/entities/EntityBuilder.h"


Entity *EntityBuilder::build(EntityManager *entityManager) {
    auto entity = entityManager->createEntity();

    if (transformComponent != nullptr) {
        entityManager->addComponent(entity, transformComponent);
    }
    if (cameraComponent != nullptr) {
        entityManager->addComponent(entity, cameraComponent);
    }
    if (highlightComponent != nullptr) {
        entityManager->addComponent<HighlightComponent>(entity, highlightComponent);
    }

    for (auto& mesh : meshComponents) {
        entityManager->addMultiComponent<MeshComponent>(entity, mesh);
    }

    if (velocityComponent != nullptr) {
        entityManager->addComponent<VelocityComponent>(entity, velocityComponent);
    }

    if (lightComponent != nullptr) {
        entityManager->addComponent<LightComponent>(entity, lightComponent);
    }

    if (massComponent != nullptr) {
        entityManager->addComponent<MassComponent>(entity, massComponent);
    }

    if (orbitComponent != nullptr) {
        entityManager->addComponent<OrbitComponent>(entity, orbitComponent);
    }

    if (selectableComponent != nullptr) {
        entityManager->addComponent<SelectableComponent>(entity, selectableComponent);
    }

    if (collisionComponent != nullptr) {
        entityManager->addComponent<CollisionComponent>(entity, collisionComponent);
    }

    return entity;
}

/**
 * Destroy entity and all components.
 */
void EntityBuilder::destroy() {
    // TODO: Delete all components that are not nullptr
    delete this;
}

EntityBuilder *EntityBuilder::withTransform(float x, float y, float z) {
    return withTransform(glm::vec3(x, y, z));
}

EntityBuilder *EntityBuilder::withTransform(glm::vec3 position) {
    if (transformComponent != nullptr) {
        throw std::runtime_error("entity already has a position");
    }
    transformComponent = new TransformComponent(position);
    return this;
}

EntityBuilder* EntityBuilder::withScale(float scale) {
    if (transformComponent == nullptr) {
        throw std::runtime_error("need a position to scale");
    }
    transformComponent->scale(scale);
    return this;
}

EntityBuilder *EntityBuilder::withMesh(std::string filename, ShaderProgram *shaderProgram) {
    auto meshes = MeshComponent::createMeshComponentsFromFile(filename, shaderProgram);
    for (auto& mesh : meshes) {
        meshComponents.push_back(mesh);
    }
    return this;
}

EntityBuilder *EntityBuilder::withMesh(std::vector<MeshComponent::Vertex> &vertices, std::vector<uint32_t> &indices,
                                       std::vector<MeshComponent::Texture> &textures, ShaderProgram *shaderProgram, GLenum mode) {
    meshComponents.push_back(new MeshComponent(vertices, indices, textures, shaderProgram, mode));
    return this;
}

EntityBuilder* EntityBuilder::withInstances(std::vector<glm::vec3> &transformations) {
    for (auto mesh : meshComponents) {
        mesh->createInstances(transformations);
    }
	return this;
}

EntityBuilder * EntityBuilder::withCamera(CameraComponent::Mode mode, CameraComponent::Type type, glm::vec3 target, glm::vec3 front, glm::vec3 up, float aspectRatio) {
    if (cameraComponent != nullptr) {
        throw std::runtime_error("entity already has a camera");
    }
    if (transformComponent == nullptr) {
        throw std::runtime_error("need a TransformComponent to initialize the CameraComponent");
    }
    cameraComponent = new CameraComponent(mode, type, target, front, up, transformComponent);
    return this;
}

EntityBuilder *EntityBuilder::withHighlight(float scaleFactor, ShaderProgram *shaderProgram) {
    if (highlightComponent != nullptr) {
        throw std::runtime_error("entity already has a highlight");
    }
    highlightComponent = new HighlightComponent(scaleFactor, shaderProgram);
    return this;
}

EntityBuilder *EntityBuilder::withVelocity(glm::vec3 velocity, std::function<void(EntityManager *, Entity *)> customUpdate) {
    velocityComponent = new VelocityComponent(velocity);
    if (customUpdate != nullptr) {
        velocityComponent->customUpdate = customUpdate;
    }
    return this;
}

EntityBuilder *EntityBuilder::withLight(LightComponent *light) {
    if (lightComponent != nullptr) {
        throw std::runtime_error("entity already has a light component");
    }
    lightComponent = light;
    return this;
}

EntityBuilder *EntityBuilder::withDirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    if (lightComponent != nullptr) {
        throw std::runtime_error("entity already has a light component");
    }
    lightComponent = LightComponent::createDirectionalLight(direction, ambient, diffuse, specular);
    return this;
}

EntityBuilder *EntityBuilder::withPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {
    if (lightComponent != nullptr) {
        throw std::runtime_error("entity already has a light component");
    }
    lightComponent = LightComponent::createPointLight(ambient, diffuse, specular, constant, linear, quadratic);
    return this;
}

EntityBuilder* EntityBuilder::withMass(double mass) {
    return withMass(new MassComponent(mass));
}

EntityBuilder* EntityBuilder::withMass(MassComponent *component) {
    if (massComponent != nullptr) {
        throw std::runtime_error("entity already has a mass");
    }
    massComponent = component;
    return this;
}

EntityBuilder* EntityBuilder::withOrbit(TransformComponent *parentTransform, double semiMajorAxis, double semiMinorAxis, double speed, double startTheta) {
    return withOrbit(new OrbitComponent(parentTransform, semiMajorAxis, semiMinorAxis, speed, startTheta));
}

EntityBuilder* EntityBuilder::withOrbit(OrbitComponent *component) {
    if (orbitComponent != nullptr) {
        throw std::runtime_error("entity already has an orbit");
    }
    orbitComponent = component;
    return this;
}

EntityBuilder* EntityBuilder::isSelectable() {
    if (selectableComponent != nullptr) {
        throw std::runtime_error("entity is already selectable");
    }
    selectableComponent = new SelectableComponent();
    return this;
}

EntityBuilder *EntityBuilder::withSphereCollision(float radius) {
    if(transformComponent == nullptr) {
        throw std::runtime_error("need a position to create a bounding sphere");
    }
    BoundingSphere boundingSphere = BoundingSphere(radius, *transformComponent);
    collisionComponent = new CollisionComponent(CollisionComponent::Sphere, boundingSphere);
    return this;
}
