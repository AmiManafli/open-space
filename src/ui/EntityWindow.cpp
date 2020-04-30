#include "cg/ui/EntityWindow.h"

EntityWindow::EntityWindow(EntityManager *entityManager, GLContext *context)
        : entityManager(entityManager), context(context), width(150), height(300) {
}

void EntityWindow::render() {
    Entity *entity = context->selectedEntity;

    if (!entity) return renderEmpty();

    ImGui::Begin("Entity Editor");
    ImGui::SetWindowSize(ImVec2(width, height));

    ImGui::Text("ID: %d", entity->id);

    ImGui::End();
}

void EntityWindow::renderEmpty() {
    ImGui::Begin("Entity Editor");
    ImGui::SetWindowSize(ImVec2(width, height));

    ImGui::Text("No entity selected.");

    ImGui::End();
}
