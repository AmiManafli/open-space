#include "ViewPlane.h"


ViewPlane::ViewPlane(glm::vec3 a, glm::vec3 b, glm::vec3 c) : a(a), b(b), c(c) {
    normal = glm::normalize(glm::cross(b - a, c - a));
    D = glm::dot(-normal, a);
}

float ViewPlane::distance(BoundingSphere &sphere) {
    return glm::dot(normal, sphere.getPosition()) + D;
}
