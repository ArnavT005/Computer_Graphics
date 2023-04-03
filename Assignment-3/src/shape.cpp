#include "shape.hpp"

void Shape::computeRayTransform() {
    mRayTransform = glm::inverse(mTransform) * glm::inverse(mWorldToCamera);
}

void Shape::computeNormalTransform() {
    mNormalTransform = glm::transpose(mRayTransform);
}

Shape::Shape() {
    mAlbedo = glm::vec3(1.0f);
    mTransform = glm::mat4(1.0f);
}

void Shape::setAlbedo(glm::vec3 albedo) {
    mAlbedo = albedo;
}

void Shape::setWorldToCamera(glm::mat4 worldToCamera) {
    mWorldToCamera = worldToCamera;
    computeRayTransform();
    computeNormalTransform();
}

void Shape::setTransform(glm::mat4 transform) {
    mTransform = transform;
    computeRayTransform();
    computeNormalTransform();
}

glm::vec3 Shape::getAlbedo() {
    return mAlbedo;
}

float Shape::getTValue() {
    return mTValue;
}

glm::vec3 Shape::getIntersectionPoint() {
    return mIntersectionPoint;
}

glm::vec3 Shape::getIntersectionNormal() {
    return mIntersectionNormal;
}
