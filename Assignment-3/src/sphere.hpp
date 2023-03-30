#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"

class Sphere: public Shape {
    private:
        glm::vec3 mAlbedo;
        glm::vec3 mCenter;
        float mRadius;
        glm::mat4 mTransform;
        float mTValue;
        glm::vec3 mIntersectionPoint;
    public:
        Sphere();
        Sphere(glm::vec3, float, glm::vec3=glm::vec3(1.0f));
        bool intersectRay(glm::vec3, glm::vec3, float, float);
        void setAlbedo(glm::vec3);
        void setCenter(glm::vec3);
        void setRadius(float);
        void setTransform(glm::mat4);
        glm::vec3 getAlbedo();
        glm::mat4 getTransform();
        float getTValue();
        glm::vec3 getIntersectionPoint();
        glm::vec3 getIntersectionNormal();
};

#endif // SPHERE_HPP