#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "object.hpp"

class Sphere : public Object {
    protected:
        glm::vec3 mCenter;
        float mRadius;
    public:
        Sphere(MaterialType, glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setCenter(glm::vec3);
        void setRadius(float);
        bool isInside(glm::vec3);
        bool intersectRay(glm::vec3, glm::vec3, float, float);
};

class SphereSource : public Sphere {
    private:
        glm::vec3 mRadiance;
    public:
        SphereSource(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setRadiance(glm::vec3);
        glm::vec3 getRadiance();
};

class DiffuseSphere : public Sphere {
    private:
        glm::vec3 mAlbedo;
    public:
        DiffuseSphere(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setAlbedo(glm::vec3);
        glm::vec3 getAlbedo();
};

class MetallicSphere : public Sphere {
    private:
        glm::vec3 mAlbedo;
        glm::vec3 mFresnelConstant;
    public:
        MetallicSphere(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setAlbedo(glm::vec3);
        void setFresnelConstant(glm::vec3);
        glm::vec3 getAlbedo();
        glm::vec3 getFresnelConstant();
        glm::vec3 getFresnelCoefficient(glm::vec3, glm::vec3);
        glm::vec3 getReflectedRayDirection(glm::vec3, glm::vec3);
};

class TransparentSphere : public Sphere {
    private:
        float mInternalRefractiveIndex, mExternalRefractiveIndex;
    public:
        TransparentSphere(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setInternalRefractiveIndex(float);
        void setExternalRefractiveIndex(float);
        float getInternalRefractiveIndex();
        float getExternalRefractiveIndex();
        float getFresnelConstant(float, float);
        float getFresnelCoefficient(float, float, glm::vec3, glm::vec3);
        glm::vec3 getReflectedRayDirection(glm::vec3, glm::vec3);
        glm::vec3 getRefractedRayDirection(float, float, glm::vec3, glm::vec3);
};

#endif // SPHERE_HPP