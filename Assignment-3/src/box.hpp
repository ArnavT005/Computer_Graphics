#ifndef BOX_HPP
#define BOX_HPP

#include "shape.hpp"

class Box: public Shape {
    private:
        glm::vec3 mMinPoint;
        glm::vec3 mMaxPoint;
    public:
        Box();
        bool intersectRay(glm::vec3, glm::vec3, float, float);
        void setMinPoint(glm::vec3);
        void setMaxPoint(glm::vec3); // it was float earlier?!
};

#endif // BOX_HPP