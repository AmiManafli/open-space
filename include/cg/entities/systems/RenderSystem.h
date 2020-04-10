#ifndef CG1_PROJECT_RENDERSYSTEM_H
#define CG1_PROJECT_RENDERSYSTEM_H

#include <cg/entities/System.h>
#include <cg/ui/UserInterface.h>

class RenderSystem : public System {
public:
    RenderSystem(EntityManager *entityManager, GLContext *context);
    ~RenderSystem();

    UserInterface* getUserInterface() { return userInterface; }

    void init() override;
    void update() override;

private:
    GLContext *context;

    UserInterface *userInterface;

    glm::vec4 clearColor;

    void renderEntities();
    void renderMesh(MeshComponent *mesh, ShaderProgram *shaderProgram, glm::mat4 model);
	void renderTexture(MeshComponent* mesh, ShaderProgram* shader);
};

#endif //CG1_PROJECT_RENDERSYSTEM_H
