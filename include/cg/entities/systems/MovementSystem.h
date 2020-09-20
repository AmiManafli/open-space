#ifndef CG1_PROJECT_MOVEMENTSYSTEM_H
#define CG1_PROJECT_MOVEMENTSYSTEM_H

#include <cg/entities/System.h>

class MovementSystem : public System {
public:
    MovementSystem(EntityManager &entityManager, GLContext &context);
    ~MovementSystem() override;

    void init();
    void update() override;

private:
    GLContext &context;
};

#endif //CG1_PROJECT_MOVEMENTSYSTEM_H
