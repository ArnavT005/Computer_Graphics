#ifndef PLANE_HPP
#define PLANE_HPP

#include "shape.hpp"

class Plane: public Shape {
    private:
        glm::vec3 mAlbedo;
        glm::vec3 mPoint;
        glm::vec3 mNormal;
        glm::mat4 mTransform;
        float mTValue;
        glm::vec3 mIntersectionPoint;
    public:
        Plane();
        Plane(glm::vec3, glm::vec3, glm::vec3 = glm::vec3(1.0f));
        bool intersectRay(glm::vec3, glm::vec3, float, float);
        void setAlbedo(glm::vec3);
        void setPoint(glm::vec3);
        void setNormal(glm::vec3);
        void setTransform(glm::mat4);
        glm::vec3 getAlbedo();
        glm::mat4 getTransform();
        float getTValue();
        glm::vec3 getIntersectionPoint();
        glm::vec3 getIntersectionNormal();
};

#endif // PLANE_HPP