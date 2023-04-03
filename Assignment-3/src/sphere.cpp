#include "sphere.hpp"

#include <iostream>

Sphere::Sphere() {}

bool Sphere::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    glm::vec3 transformedOrigin(mRayTransform * glm::vec4(origin, 1.0f));
    glm::vec3 transformedDirection(mRayTransform * glm::vec4(direction, 0.0f));
    float a = glm::dot(transformedDirection, transformedDirection);
    float b = 2 * glm::dot(transformedOrigin - mCenter, transformedDirection);
    float c = glm::dot(transformedOrigin - mCenter, transformedOrigin - mCenter) - mRadius * mRadius;
    if (b * b - 4 * a * c < 0) {
        return false;
    }
    float root[2];
    root[0] = (- b + glm::sqrt(b * b - 4 * a * c)) / (2 * a);
    root[1] = (- b - glm::sqrt(b * b - 4 * a * c)) / (2 * a);
    if (root[0] < tMin || root[0] > tMax) {
        if (tMin <= root[1] && root[1] <= tMax) {
            mTValue = root[1];
            mIntersectionPoint = origin + mTValue * direction;
            mIntersectionNormal = glm::normalize(mNormalTransform * glm::vec4(transformedOrigin + mTValue * transformedDirection - mCenter, 0.0f));
            return true;
        }
        return false;
    }
    if (tMin <= root[1] && root[1] <= tMax) {
        root[0] = glm::min(root[0], root[1]);
    }
    mTValue = root[0];
    mIntersectionPoint = origin + mTValue * direction;
    mIntersectionNormal = glm::normalize(mNormalTransform * glm::vec4(transformedOrigin + mTValue * transformedDirection - mCenter, 0.0f));
    return true;
}

void Sphere::setCenter(glm::vec3 center) {
    mCenter = center;
}

void Sphere::setRadius(float radius) {
    mRadius = radius;
}