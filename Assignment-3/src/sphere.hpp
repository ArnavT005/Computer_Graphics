#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"

class Sphere: public Shape {
    private:
        glm::vec3 mCenter;
        float mRadius;
    public:
        Sphere();
        bool intersectRay(glm::vec3, glm::vec3, float, float);
        void setCenter(glm::vec3);
        void setRadius(float);
};

#endif // SPHERE_HPP