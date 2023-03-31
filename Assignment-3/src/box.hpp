#ifndef BOX_HPP
#define BOX_HPP

#include "shape.hpp"

class Box: public Shape {
    private:
        glm::vec3 mAlbedo;
        glm::vec3 mMinPoint;
        glm::vec3 mMaxPoint;
        glm::mat4 mTransform;
        float mTValue;
        glm::vec3 mIntersectionPoint;
    public:
        Box();
        Box(glm::vec3, glm::vec3, glm::vec3 = glm::vec3(1.0f));
        bool intersectRay(glm::vec3, glm::vec3, float, float);
        void setAlbedo(glm::vec3);
        void setMinPoint(glm::vec3);
        void setMaxPoint(float);
        void setTransform(glm::mat4);
        glm::vec3 getAlbedo();
        glm::mat4 getTransform();
        float getTValue();
        glm::vec3 getIntersectionPoint();
        glm::vec3 getIntersectionNormal();
};

#endif // BOX_HPP