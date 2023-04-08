#include "plane.hpp"

Plane::Plane(MaterialType material, glm::mat4 transform, glm::mat4 worldToCamera) 
    : Object(ShapeType::PLANE, material, transform, worldToCamera) {}

void Plane::setPoint(glm::vec3 point) {
    mPoint = point;
}

void Plane::setNormal(glm::vec3 normal) {
    mNormal = glm::normalize(normal);
}

bool Plane::isInside(glm::vec3 point) {
    mInside = glm::dot(point - mPoint, mNormal) < 0;
    return mInside;
}

bool Plane::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    glm::vec3 transformedOrigin(mRayTransform * glm::vec4(origin, 1.0f));
    glm::vec3 transformedDirection(mRayTransform * glm::vec4(direction, 0.0f));
    float normalSign = isInside(transformedOrigin + 0.001f * transformedDirection) ? -1 : 1;
    if (glm::dot(transformedDirection, mNormal) != 0) {
        mTValue = glm::dot(mNormal, mPoint - transformedOrigin) / glm::dot(mNormal, transformedDirection);
        mIntersectionPoint = origin + mTValue * direction;
        mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(mNormal, 0.0f)));
        return (tMin <= mTValue && mTValue <= tMax);
    }
    if (glm::dot(mNormal, transformedOrigin - mPoint) == 0) {
        mTValue = glm::max(tMin, 0.0f);
        mIntersectionPoint = origin + mTValue * direction;
        mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(mNormal, 0.0f)));
        return true;
    }
    return false;
}

DiffusePlane::DiffusePlane(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Plane(MaterialType::DIFFUSE, transform, worldToCamera) {}

void DiffusePlane::setAlbedo(glm::vec3 albedo) {
    mAlbedo = albedo;
}

glm::vec3 DiffusePlane::getAlbedo() {
    return mAlbedo;
}

MetallicPlane::MetallicPlane(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Plane(MaterialType::METALLIC, transform, worldToCamera) {}

void MetallicPlane::setAlbedo(glm::vec3 albedo) {
    mAlbedo = albedo;
}

void MetallicPlane::setFresnelConstant(glm::vec3 fresnelConstant) {
    mFresnelConstant = fresnelConstant;
}

glm::vec3 MetallicPlane::getAlbedo() {
    return mAlbedo;
}

glm::vec3 MetallicPlane::getFresnelConstant() {
    return mFresnelConstant;
}

glm::vec3 MetallicPlane::getFresnelCoefficient(glm::vec3 direction, glm::vec3 normal) {
    direction = - glm::normalize(direction);
    return mFresnelConstant + (1.0f - mFresnelConstant) * powf((1 - glm::dot(direction, normal)), 5);
}

glm::vec3 MetallicPlane::getReflectedRayDirection(glm::vec3 direction, glm::vec3 normal) {
    return glm::normalize(direction - 2.0f * glm::dot(direction, normal) * normal);
}