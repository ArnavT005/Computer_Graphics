#include "sphere.hpp"

Sphere::Sphere(MaterialType material, glm::mat4 transform, glm::mat4 worldToCamera) 
    : Object(ShapeType::SPHERE, material, transform, worldToCamera) {}

void Sphere::setCenter(glm::vec3 center) {
    mCenter = center;
}

void Sphere::setRadius(float radius) {
    mRadius = radius;
}

bool Sphere::isInside(glm::vec3 point) {
    mInside = glm::dot(point - mCenter, point - mCenter) < mRadius * mRadius;
    return mInside;
}

bool Sphere::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    glm::vec3 transformedOrigin(mRayTransform * glm::vec4(origin, 1.0f));
    glm::vec3 transformedDirection(mRayTransform * glm::vec4(direction, 0.0f));
    float a = glm::dot(transformedDirection, transformedDirection);
    float b = 2 * glm::dot(transformedOrigin - mCenter, transformedDirection);
    float c = glm::dot(transformedOrigin - mCenter, transformedOrigin - mCenter) - mRadius * mRadius;
    if (b * b - 4 * a * c < 0) {
        return false;
    }
    float normalSign = isInside(transformedOrigin + 0.001f * transformedDirection) ? -1 : 1;
    float root[2];
    root[0] = (- b + glm::sqrt(b * b - 4 * a * c)) / (2 * a);
    root[1] = (- b - glm::sqrt(b * b - 4 * a * c)) / (2 * a);
    if (root[0] < tMin || root[0] > tMax) {
        if (tMin <= root[1] && root[1] <= tMax) {
            mTValue = root[1];
            mIntersectionPoint = origin + mTValue * direction;
            mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(transformedOrigin + mTValue * transformedDirection - mCenter, 0.0f)));
            return true;
        }
        return false;
    }
    if (tMin <= root[1] && root[1] <= tMax) {
        root[0] = glm::min(root[0], root[1]);
    }
    mTValue = root[0];
    mIntersectionPoint = origin + mTValue * direction;
    mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(transformedOrigin + mTValue * transformedDirection - mCenter, 0.0f)));
    return true;
}

SphereSource::SphereSource(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Sphere(MaterialType::LIGHT, transform, worldToCamera) {}

void SphereSource::setRadiance(glm::vec3 radiance) {
    mRadiance = radiance;
}

glm::vec3 SphereSource::getRadiance() {
    return mRadiance;
}

DiffuseSphere::DiffuseSphere(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Sphere(MaterialType::DIFFUSE, transform, worldToCamera) {}

void DiffuseSphere::setAlbedo(glm::vec3 albedo) {
    mAlbedo = albedo;
}

glm::vec3 DiffuseSphere::getAlbedo() {
    return mAlbedo;
}

MetallicSphere::MetallicSphere(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Sphere(MaterialType::METALLIC, transform, worldToCamera) {}

void MetallicSphere::setFresnelConstant(glm::vec3 fresnelConstant) {
    mFresnelConstant = fresnelConstant;
}

glm::vec3 MetallicSphere::getFresnelConstant() {
    return mFresnelConstant;
}

glm::vec3 MetallicSphere::getFresnelCoefficient(glm::vec3 direction, glm::vec3 normal) {
    direction = - glm::normalize(direction);
    return mFresnelConstant + (1.0f - mFresnelConstant) * powf((1 - glm::dot(direction, normal)), 5);
}

glm::vec3 MetallicSphere::getReflectedRayDirection(glm::vec3 direction, glm::vec3 normal) {
    return glm::normalize(direction - 2.0f * glm::dot(direction, normal) * normal);
}

TransparentSphere::TransparentSphere(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Sphere(MaterialType::TRANSPARENT, transform, worldToCamera) {}

void TransparentSphere::setInternalRefractiveIndex(float internalRefractiveIndex) {
    mInternalRefractiveIndex = internalRefractiveIndex;
}

void TransparentSphere::setExternalRefractiveIndex(float externalRefractiveIndex) {
    mExternalRefractiveIndex = externalRefractiveIndex;
}

float TransparentSphere::getInternalRefractiveIndex() {
    return mInternalRefractiveIndex;
}

float TransparentSphere::getExternalRefractiveIndex() {
    return mExternalRefractiveIndex;
}

float TransparentSphere::getFresnelConstant(float outside, float inside) {
    return powf((outside - inside) / (outside + inside), 2);
}

float TransparentSphere::getFresnelCoefficient(float outside, float inside, glm::vec3 direction, glm::vec3 normal) {
    float fresnelConstant = getFresnelConstant(outside, inside);
    glm::vec3 transmission = getRefractedRayDirection(outside, inside, direction, normal);
    direction = - glm::normalize(direction);
    if (glm::dot(direction, normal) < glm::dot(-transmission, normal)) {
        return fresnelConstant + (1.0f - fresnelConstant) * powf(1 - glm::dot(direction, normal), 5);
    } else {
        return fresnelConstant + (1.0f - fresnelConstant) * powf(1 - glm::dot(-transmission, normal), 5);
    }
}

glm::vec3 TransparentSphere::getReflectedRayDirection(glm::vec3 direction, glm::vec3 normal) {
    return glm::normalize(direction - 2.0f * glm::dot(direction, normal) * normal);
}

glm::vec3 TransparentSphere::getRefractedRayDirection(float outside, float inside, glm::vec3 direction, glm::vec3 normal) {
    float relative = outside / inside;
    direction = - glm::normalize(direction);
    if (1 - powf(relative, 2) * (1 - powf(glm::dot(direction, normal), 2)) < 0) {
        return glm::vec3(0);
    }
    return glm::normalize((relative * glm::dot(direction, normal) - sqrtf(1 - powf(relative, 2) * (1 - powf(glm::dot(direction, normal), 2)))) * normal - relative * direction);
}
