#ifndef BOX_HPP
#define BOX_HPP

#include "object.hpp"

class Box : public Object {
    private:
        glm::vec3 mMinPoint;
        glm::vec3 mMaxPoint;
    public:
        Box(MaterialType, glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setPoints(glm::vec3, glm::vec3);
        bool isInside(glm::vec3);
        bool intersectRay(glm::vec3, glm::vec3, float, float);
};

class BoxSource : public Box {
    private:
        glm::vec3 mRadiance;
    public:
        BoxSource(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setRadiance(glm::vec3);
        glm::vec3 getRadiance();
};

class DiffuseBox : public Box {
    private:
        glm::vec3 mAlbedo;
    public:
        DiffuseBox(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setAlbedo(glm::vec3);
        glm::vec3 getAlbedo();
};

class MetallicBox : public Box {
    private:
        glm::vec3 mAlbedo;
        glm::vec3 mFresnelConstant;
    public:
        MetallicBox(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setAlbedo(glm::vec3);
        void setFresnelConstant(glm::vec3);
        glm::vec3 getAlbedo();
        glm::vec3 getFresnelConstant();
        glm::vec3 getFresnelCoefficient(glm::vec3, glm::vec3);
        glm::vec3 getReflectedRayDirection(glm::vec3, glm::vec3);
};

class TransparentBox : public Box {
    private:
        float mInternalRefractiveIndex, mExternalRefractiveIndex;
    public:
        TransparentBox(glm::mat4 = glm::mat4(1.0f), glm::mat4 = glm::mat4(1.0f));
        void setInternalRefractiveIndex(float);
        void setExternalRefractiveIndex(float);
        float getInternalRefractiveIndex();
        float getExternalRefractiveIndex();
        float getFresnelConstant(float, float);
        float getFresnelCoefficient(float, float, glm::vec3, glm::vec3);
        glm::vec3 getReflectedRayDirection(glm::vec3, glm::vec3);
        glm::vec3 getRefractedRayDirection(float, float, glm::vec3, glm::vec3);
};

#endif // BOX_HPP