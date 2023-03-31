#include "plane.hpp"

Plane::Plane() {
    mAlbedo = glm::vec3(1.0f);
    mPoint = glm::vec3(0.0f);
    mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
    mTransform = glm::mat4(1.0f);
    mIntersectionPoint = glm::vec3(-1.0f);
}

Plane::Plane(glm::vec3 point, glm::vec3 normal, glm::vec3 albedo) {
    mAlbedo = albedo;
    mPoint = point;
    mNormal = glm::normalize(normal);
    mTransform = glm::mat4(1.0f);
    mIntersectionPoint = glm::vec3(-1.0f);
}

bool Plane::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    if (glm::dot(direction, mNormal) != 0) {
        mTValue = glm::dot(mNormal, mPoint - origin) / glm::dot(mNormal, direction);
        mIntersectionPoint = origin + mTValue * direction;
        return (tMin <= mTValue && mTValue <= tMax);
    }
    if (glm::dot(mNormal, origin - mPoint) == 0) {
        mTValue = glm::max(tMin, 0.0f);
        mIntersectionPoint = origin + mTValue * direction;
        return true;
    }
    return false;
}

void Plane::setAlbedo(glm::vec3 albedo) {
    mAlbedo = albedo;
}

void Plane::setPoint(glm::vec3 point) {
    mPoint = point;
}

void Plane::setNormal(glm::vec3 normal) {
    mNormal = glm::normalize(normal);
}

void Plane::setTransform(glm::mat4 transform) {
    mTransform = transform;
}

glm::vec3 Plane::getAlbedo() {
    return mAlbedo;
}

glm::mat4 Plane::getTransform() {
    return mTransform;
}

float Plane::getTValue() {
    return mTValue;
}

glm::vec3 Plane::getIntersectionPoint() {
    return mIntersectionPoint;
}

glm::vec3 Plane::getIntersectionNormal() {
    return mNormal;
}