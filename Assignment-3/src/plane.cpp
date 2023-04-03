#include "plane.hpp"

Plane::Plane() {}

bool Plane::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    glm::vec3 transformedOrigin(mRayTransform * glm::vec4(origin, 1.0f));
    glm::vec3 transformedDirection(mRayTransform * glm::vec4(direction, 0.0f));
    if (glm::dot(transformedDirection, mNormal) != 0) {
        mTValue = glm::dot(mNormal, mPoint - transformedOrigin) / glm::dot(mNormal, transformedDirection);
        mIntersectionPoint = origin + mTValue * direction;
        mIntersectionNormal = glm::normalize(mNormalTransform * glm::vec4(mNormal, 0.0f));
        return (tMin <= mTValue && mTValue <= tMax);
    }
    if (glm::dot(mNormal, transformedOrigin - mPoint) == 0) {
        mTValue = glm::max(tMin, 0.0f);
        mIntersectionPoint = origin + mTValue * direction;
        mIntersectionNormal = glm::normalize(mNormalTransform * glm::vec4(mNormal, 0.0f));
        return true;
    }
    return false;
}

void Plane::setPoint(glm::vec3 point) {
    mPoint = point;
}

void Plane::setNormal(glm::vec3 normal) {
    mNormal = glm::normalize(normal);
}