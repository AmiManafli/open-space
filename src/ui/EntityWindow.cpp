#include "cg/ui/EntityWindow.h"

GLenum meshModeStringToGLenum(const char *mode) {
    if (strcmp(mode, "Triangles") == 0) {
        return GL_TRIANGLES;
    } else if (strcmp(mode, "Lines") == 0) {
        return GL_LINES;
    } else if (strcmp(mode, "Points") == 0) {
        return GL_POINTS;
    }
    throw std::runtime_error("Invalid mesh mode");
}

std::string meshModeEnumToString(GLenum mode) {
    if (mode == GL_TRIANGLES) {
        return "Triangles";
    } else if (mode == GL_LINES) {
        return "Lines";
    } else if (mode == GL_POINTS) {
        return "Points";
    }
    throw std::runtime_error("Invalid mesh mode");
}

EntityWindow::EntityWindow(EntityManager *entityManager, GLContext *context)
        : entityManager(entityManager), context(context), width(300), height(0) {
    meshModes = { "Triangles", "Lines", "Points" };
}

void EntityWindow::render() {
    Entity *entity = context->selectedEntity;

    if (!entity) return renderEmpty();

    ImGui::Begin("Entity Editor");
    ImGui::SetWindowSize(ImVec2(width, height));

    renderInfo(entity);

    // Components
    std::vector<MeshComponent *> meshComponents;
    auto meshes = entityManager->getMeshComponents(entity->id);
    for (auto i = meshes.first; i != meshes.second; i++) {
        meshComponents.emplace_back(i->second);
    }

    renderTransformComponent(entityManager->getComponent<TransformComponent>(entity));
    renderMassComponent(entityManager->getMassComponent(entity));
    renderOrbitComponent(entityManager->getOrbitComponent(entity));
    renderVelocityComponent(entityManager->getVelocityComponent(entity));
    renderCameraComponent(entityManager->getCameraComponent(entity));
    renderMeshComponents(meshComponents);
    renderHighlightComponent(entityManager->getHighlightComponent(entity));

    ImGui::End();
}

void EntityWindow::renderEmpty() {
    ImGui::Begin("Entity Editor");
    ImGui::SetWindowSize(ImVec2(width, height));

    ImGui::Text("No entity selected.");

    ImGui::End();
}

void EntityWindow::renderInfo(Entity *entity) {
    ImGui::Text("ID: %d", entity->id);
}

void EntityWindow::renderTransformComponent(TransformComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Transform Component")) {
        ImGui::DragFloat3("Position", glm::value_ptr(component->position), 0.1, -10000, 10000);
        ImGui::DragFloat3("Rotation", glm::value_ptr(component->rotation), 0.1, -10000, 10000);
        ImGui::DragFloat3("Scaling", glm::value_ptr(component->scaling), 0.1, -10000, 10000);
    }
}

void EntityWindow::renderMassComponent(MassComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Mass Component")) {
        ImGui::DragFloat("Mass (kg)", reinterpret_cast<float *>(&component->mass), 0.1, 0, 10000);
    }
}

void EntityWindow::renderOrbitComponent(OrbitComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Orbit Component")) {
        ImGui::Text("Eccentricity: %f\n", component->eccentricity);
        ImGui::Text("Width: %f\n", component->width);
        ImGui::Text("Height: %f\n", component->height);

        if (ImGui::DragFloat("Semi-Major Axis", &component->semiMajorAxis, 0.1, component->semiMinorAxis, 10000)) {
            component->update(component->semiMajorAxis, component->semiMinorAxis, component->speed);
        }
        if (ImGui::DragFloat("Semi-Minor Axis", &component->semiMinorAxis, 0.1, 0.001, component->semiMajorAxis)) {
            component->update(component->semiMajorAxis, component->semiMinorAxis, component->speed);
        }

        ImGui::DragFloat("Orbit Speed", &component->speed, 0.1, 0, 10000);
        ImGui::DragFloat("Angle (radians)", &component->theta, 0.1, 0, 10000);
    }
}

void EntityWindow::renderVelocityComponent(VelocityComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Velocity Component")) {
        ImGui::DragFloat3("Position", glm::value_ptr(component->position), 0.1, -10000, 10000);
        ImGui::DragFloat3("Gravity", glm::value_ptr(component->gravity), 0.1, -10000, 10000);
        ImGui::DragFloat3("Rotation", glm::value_ptr(component->rotation), 0.1, -10000, 10000);
        ImGui::DragFloat3("Rotation Point", glm::value_ptr(component->rotationPoint), 0.1, -10000, 10000);
        ImGui::DragFloat3("Scaling", glm::value_ptr(component->scaling), 0.1, 0, 10000);
    }
}

void EntityWindow::renderCameraComponent(CameraComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Camera Component")) {
        std::string mode;
        if (component->mode == CameraComponent::Free) {
            mode = "Free";
        } else {
            mode = "Unknown";
        }
        ImGui::Text("Mode: %s", mode.c_str());

        std::string type;
        if (component->type == CameraComponent::Orthographic) {
            type = "Orthographic";
        } else if (component->type == CameraComponent::Perspective) {
            type = "Perspective";
        } else {
            type = "Unknown";
        }
        ImGui::Text("Type: %s", type.c_str());

        ImGui::DragFloat("Zoom", &component->zoom, 0.1, 0, 100);
        ImGui::DragFloat3("Target", glm::value_ptr(component->target), 0.1, -10000, 10000);

        ImGui::Text("Front: %s", glm::to_string(component->front).c_str());
        ImGui::Text("Up: %s", glm::to_string(component->up).c_str());
        ImGui::Text("Right: %s", glm::to_string(component->right).c_str());

        ImGui::DragFloat("Yaw", &component->yaw, 0.1, -10000, 10000);
        ImGui::DragFloat("Pitch", &component->pitch, 0.1, -10000, 10000);
        ImGui::DragFloat("Roll", &component->roll, 0.1, -10000, 10000);

        ImGui::DragFloat("Mouse sensitivity", &component->mouseSensitivity, 0.1, -10000, 10000);
        ImGui::DragFloat("Movement speed", &component->movementSpeed, 0.1, -10000, 10000);
    }
}

void EntityWindow::renderMeshComponents(std::vector<MeshComponent *> components) {
    if (components.empty()) return;

    if (ImGui::CollapsingHeader("Mesh Components")) {
        int index = 0;
        for (MeshComponent *mesh : components) {
            std::string name = "Mesh " + std::to_string(index++);
            if (ImGui::TreeNode(name.c_str())) {
                std::string currentMode = meshModeEnumToString(mesh->mode);
                if (ImGui::BeginCombo("Mode", currentMode.c_str())) {
                    for (auto mode : meshModes) {
                        bool isSelected = strcmp(currentMode.c_str(), mode) == 0;
                        if (ImGui::Selectable(mode, isSelected)) {
                            mesh->mode = meshModeStringToGLenum(mode);
                        }
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::ColorPicker3("Color", glm::value_ptr(mesh->color))) {
                    mesh->setColor(mesh->color);
                }

                ImGui::Text("Instance count: %d", mesh->instances);
                ImGui::Text("Vertices: %llu", mesh->vertices.size());
                ImGui::Text("Indices: %llu", mesh->indices.size());
                ImGui::Text("Textures: %llu", mesh->textures.size());

                int subdivisions = mesh->subdivision;
                if (ImGui::SliderInt("Subdivisions", &subdivisions, 0, 7)) {
                    mesh->subdivision = subdivisions;
                    mesh->subdivide(subdivisions);
                }

                ImGui::TreePop();
                if (components.size() != index) {
                    ImGui::Separator();
                }
            }
        }
    }
}

void EntityWindow::renderHighlightComponent(HighlightComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Highlight Component")) {
        ImGui::DragFloat("Size", &component->size, 0.1, 0, 100);
    }
}

