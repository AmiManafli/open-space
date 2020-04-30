#ifndef CG1_PROJECT_INPUTSYSTEM_H
#define CG1_PROJECT_INPUTSYSTEM_H

#include <cg/GLHeader.h>
#include <cg/entities/System.h>
#include "cg/SpaceshipControl.h"


class InputSystem : public System {
public:
    InputSystem(EntityManager *entityManager, GLContext *context);

    void init() override;
    void update() override;
    void createSpaceshipControl(Entity* spaceship, Entity* camera);


    bool isKeyDown(int key);
    bool isKeyUp(int key);
    bool isKeyPressed(int key);

private:
    GLContext *context;

    SpaceshipControl* spaceshipControl;
    bool processedMouse = false;
    double lastMouseX, lastMouseY;
    std::unordered_map<uint32_t, uint32_t> keysDown;
    std::unordered_map<uint32_t, uint32_t> mouseButtonsDown;

    void moveCamera(Entity *camera, CameraComponent::Direction direction, float deltaTime);
    Entity* getClickedEntity(double mouseX, double mouseY);

    bool isRayInSphere(Entity* entity, glm::vec3 origin, glm::vec3 ray);
    void selectEntity(Entity *entity);

    static void mousePositionCallback(GLFWwindow* window, double x, double y);
    static void processMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
    static void processMouseButton(GLFWwindow* window, int button, int action, int mods);
};

#endif //CG1_PROJECT_INPUTSYSTEM_H
