#include "sphere.hpp"

#include <iostream>

Sphere::Sphere() {
    mAlbedo = glm::vec3(1.0f);
    mCenter = glm::vec3(0.0f);
    mRadius = 1.0f;
    mTransform = glm::mat4(1.0f);
    mIntersectionPoint = glm::vec3(-1.0f);
}

Sphere::Sphere(glm::vec3 center, float radius, glm::vec3 albedo) {
    mAlbedo = albedo;
    mCenter = center;
    mRadius = radius;
    mTransform = glm::mat4(1.0f);
    mIntersectionPoint = glm::vec3(-1.0f);
}

bool Sphere::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    float a = glm::dot(direction, direction);
    float b = 2 * glm::dot(origin - mCenter, direction);
    float c = glm::dot(origin - mCenter, origin - mCenter) - mRadius * mRadius;
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
            return true;
        }
        return false;
    }
    if (tMin <= root[1] && root[1] <= tMax) {
        root[0] = glm::min(root[0], root[1]);
    }
    mTValue = root[0];
    mIntersectionPoint = origin + mTValue * direction;
    return true;
}

void Sphere::setAlbedo(glm::vec3 albedo) {
    mAlbedo = albedo;
}

void Sphere::setCenter(glm::vec3 center) {
    mCenter = center;
}

void Sphere::setRadius(float radius) {
    mRadius = radius;
}

void Sphere::setTransform(glm::mat4 transform) {
    mTransform = transform;
}

glm::vec3 Sphere::getAlbedo() {
    return mAlbedo;
}

glm::mat4 Sphere::getTransform() {
    return mTransform;
}

float Sphere::getTValue() {
    return mTValue;
}

glm::vec3 Sphere::getIntersectionPoint() {
    return mIntersectionPoint;
}

glm::vec3 Sphere::getIntersectionNormal() {
    return glm::normalize(mIntersectionPoint - mCenter);
}