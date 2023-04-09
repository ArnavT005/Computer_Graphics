#ifndef PLANE_HPP
#define PLANE_HPP

#include "object.hpp"

class Plane : public Object {
    protected:
        glm::vec3 mPoint;
        glm::vec3 mNormal;
    public:
        Plane(MaterialType, glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setPoint(glm::vec3);
        void setNormal(glm::vec3);
        bool isInside(glm::vec3);
        bool intersectRay(glm::vec3, glm::vec3, float, float);
};

class DiffusePlane : public Plane {
    private:
        glm::vec3 mAlbedo;
    public:
        DiffusePlane(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setAlbedo(glm::vec3);
        glm::vec3 getAlbedo();
};

class MetallicPlane : public Plane {
    private:
        glm::vec3 mFresnelConstant;
    public:
        MetallicPlane(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setFresnelConstant(glm::vec3);
        glm::vec3 getFresnelConstant();
        glm::vec3 getFresnelCoefficient(glm::vec3, glm::vec3);
        glm::vec3 getReflectedRayDirection(glm::vec3, glm::vec3);
};

#endif // PLANE_HPP