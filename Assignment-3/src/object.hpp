#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp>

enum ShapeType {
    SPHERE,
    PLANE,
    BOX
};

enum MaterialType {
    LIGHT,
    DIFFUSE,
    METALLIC,
    TRANSPARENT
};

class Object {
    protected:
        ShapeType mShape;
        MaterialType mMaterial;
        glm::mat4 mTransform;
        glm::mat4 mWorldToCamera;
        glm::mat4 mRayTransform;
        glm::mat4 mNormalTransform;
        float mTValue;
        glm::vec3 mIntersectionPoint;
        glm::vec3 mIntersectionNormal;
        bool mInside;
        void computeInverseTransforms();
    public:
        Object(ShapeType, MaterialType, glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setTransform(glm::mat4);
        void setWorldToCamera(glm::mat4);
        ShapeType getShape();
        MaterialType getMaterial();
        float getTValue();
        glm::vec3 getIntersectionPoint();
        glm::vec3 getIntersectionNormal();
        bool getInside();
        virtual bool isInside(glm::vec3) = 0;
        virtual bool intersectRay(glm::vec3, glm::vec3, float, float) = 0;
};

#endif // OBJECT_HPP