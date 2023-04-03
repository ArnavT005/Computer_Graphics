#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shape {
    protected:
        glm::vec3 mAlbedo;
        glm::mat4 mWorldToCamera;
        glm::mat4 mTransform;
        glm::mat4 mRayTransform;
        glm::mat4 mNormalTransform;
        float mTValue;
        glm::vec3 mIntersectionPoint;
        glm::vec3 mIntersectionNormal;
        void computeRayTransform();
        void computeNormalTransform();
    public:
        Shape();
        virtual bool intersectRay(glm::vec3, glm::vec3, float, float) = 0;
        void setAlbedo(glm::vec3);
        void setWorldToCamera(glm::mat4);
        void setTransform(glm::mat4);
        glm::vec3 getAlbedo();
        float getTValue();
        glm::vec3 getIntersectionPoint();
        glm::vec3 getIntersectionNormal();
};

#endif // SHAPE_HPP