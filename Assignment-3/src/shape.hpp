#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shape {
    public:
        virtual bool intersectRay(glm::vec3, glm::vec3, float, float) = 0;
        virtual float getTValue() = 0;
        virtual glm::vec3 getIntersectionPoint() = 0;
        virtual glm::vec3 getIntersectionNormal() = 0;
};

#endif // SHAPE_HPP