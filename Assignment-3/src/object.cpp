#include "object.hpp"

void Object::computeInverseTransforms() {
    mRayTransform = glm::inverse(mTransform) * glm::inverse(mWorldToCamera);
    mNormalTransform = glm::transpose(mRayTransform);
}

Object::Object(ShapeType shape, MaterialType material, glm::mat4 transform, glm::mat4 worldToCamera) {
    mShape = shape;
    mMaterial = material;
    mTransform = transform;
    mWorldToCamera = worldToCamera;
    computeInverseTransforms();
}

void Object::setTransform(glm::mat4 transform) {
    mTransform = transform;
    computeInverseTransforms();
}

void Object::setWorldToCamera(glm::mat4 worldToCamera) {
    mWorldToCamera = worldToCamera;
    computeInverseTransforms();
}

ShapeType Object::getShape() {
    return mShape;
}

MaterialType Object::getMaterial() {
    return mMaterial;
}

float Object::getTValue() {
    return mTValue;
}

glm::vec3 Object::getIntersectionPoint() {
    return mIntersectionPoint;
}

glm::vec3 Object::getIntersectionNormal() {
    return mIntersectionNormal;
}

bool Object::getInside() {
    return mInside;
}