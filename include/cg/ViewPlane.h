#ifndef CG1_PROJECT_VIEWPLANE_H
#define CG1_PROJECT_VIEWPLANE_H

#include <cg/GLHeader.h>
#include <cg/collision/BoundingSphere.h>


class ViewPlane {
public:
    ViewPlane() = default;
    ViewPlane(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    float distance(BoundingSphere &sphere);

    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 normal;
    float D;
};


#endif //CG1_PROJECT_VIEWPLANE_H
