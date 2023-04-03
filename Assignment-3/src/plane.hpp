#ifndef PLANE_HPP
#define PLANE_HPP

#include "shape.hpp"

class Plane: public Shape {
    private:
        glm::vec3 mPoint;
        glm::vec3 mNormal;
    public:
        Plane();
        bool intersectRay(glm::vec3, glm::vec3, float, float);
        void setPoint(glm::vec3);
        void setNormal(glm::vec3);
};

#endif // PLANE_HPP