#include "cg/ui/EntityWindow.h"

EntityWindow::EntityWindow(EntityManager *entityManager, GLContext *context)
        : entityManager(entityManager), context(context), width(300), height(0) {
}

void EntityWindow::render() {
    Entity *entity = context->selectedEntity;

    if (!entity) return renderEmpty();

    ImGui::Begin("Entity Editor");
    ImGui::SetWindowSize(ImVec2(width, height));

    renderInfo(entity);

    // Components
    renderTransformComponent(entityManager->getTransformComponent(entity));
    renderMassComponent(entityManager->getMassComponent(entity));
    renderOrbitComponent(entityManager->getOrbitComponent(entity));
    renderVelocityComponent(entityManager->getVelocityComponent(entity));
    renderCameraComponent(entityManager->getCameraComponent(entity));
//    renderMeshComponents(entityManager->getMeshComponents(entity));
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
        ImGui::DragFloat3("Position", glm::value_ptr(component->position), 0.1f, -10000, 10000);
        ImGui::DragFloat3("Rotation", glm::value_ptr(component->rotation), 0.1f, -10000, 10000);
        ImGui::DragFloat3("Scaling", glm::value_ptr(component->scaling), 0.1f, -10000, 10000);
    }
}

void EntityWindow::renderMassComponent(MassComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Mass Component")) {
        ImGui::DragFloat("Mass (kg)", reinterpret_cast<float *>(&component->mass), 0.1f, 0, 10000);
    }
}

void EntityWindow::renderOrbitComponent(OrbitComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Orbit Component")) {
        ImGui::Text("Eccentricity: %f\n", component->eccentricity);
        ImGui::Text("Width: %f\n", component->width);
        ImGui::Text("Height: %f\n", component->height);

        if (ImGui::DragFloat("Semi-Major Axis", &component->semiMajorAxis, 0.1f, 0, 10000)) {
            component->update(component->parentPosition, component->semiMajorAxis, component->semiMinorAxis, component->speed);
        }
        if (ImGui::DragFloat("Semi-Minor Axis", &component->semiMinorAxis, 0.1f, 0, 10000)) {
            component->update(component->parentPosition, component->semiMajorAxis, component->semiMinorAxis, component->speed);
        }

        ImGui::DragFloat("Orbit Speed", &component->speed, 0.1f, 0, 10000);
        ImGui::DragFloat("Angle (radians)", &component->theta, 0.1f, 0, 10000);
    }
}

void EntityWindow::renderVelocityComponent(VelocityComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Velocity Component")) {
    }
}

void EntityWindow::renderCameraComponent(CameraComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Camera Component")) {
    }
}

void EntityWindow::renderMeshComponents(MeshComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Mesh Components")) {
    }
}

void EntityWindow::renderHighlightComponent(HighlightComponent *component) {
    if (!component) return;

    if (ImGui::CollapsingHeader("Highlight Component")) {
    }
}

