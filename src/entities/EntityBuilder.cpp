#include "cg/entities/EntityBuilder.h"


Entity *EntityBuilder::build(EntityManager *entityManager) {
    auto entity = entityManager->createEntity();

    if (transformComponent != nullptr) {
        entityManager->addTransformComponent(entity->id, transformComponent);
    }
    if (cameraComponent != nullptr) {
        entityManager->addCameraComponent(entity->id, cameraComponent);
    }
    if (highlightComponent != nullptr) {
        entityManager->addHighlightComponent(entity->id, highlightComponent);
    }

    for (auto& mesh : meshComponents) {
        entityManager->addMeshComponent(entity->id, mesh);
    }

    if (velocityComponent != nullptr) {
        entityManager->addVelocityComponent(entity->id, velocityComponent);
    }

    if (lightComponent != nullptr) {
        entityManager->addLightComponent(entity->id, lightComponent);
    }

    if (massComponent != nullptr) {
        entityManager->addMassComponent(entity->id, massComponent);
    }

    if (orbitComponent != nullptr) {
        entityManager->addOrbitComponent(entity->id, orbitComponent);
    }

    return entity;
}

/**
 * Destroy entity and all components.
 */
void EntityBuilder::destroy() {
    for (auto mesh : meshComponents) {
        delete mesh;
    }
    delete transformComponent;
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

EntityBuilder *EntityBuilder::withVelocity(glm::vec3 velocity, std::function<void(EntityManager *, uint32_t)> customUpdate) {
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

EntityBuilder* EntityBuilder::withOrbit(glm::vec3 parentPosition, double semiMajorAxis, double semiMinorAxis, double speed, double startTheta) {
    return withOrbit(new OrbitComponent(parentPosition, semiMajorAxis, semiMinorAxis, speed, startTheta));
}

EntityBuilder* EntityBuilder::withOrbit(OrbitComponent *component) {
    if (orbitComponent != nullptr) {
        throw std::runtime_error("entity already has an orbit");
    }
    orbitComponent = component;

//    transformComponent->position += orbitComponent->startPosition;
//
//    printf("Start position: %s\n", glm::to_string(transformComponent->position).c_str());

    return this;
}
