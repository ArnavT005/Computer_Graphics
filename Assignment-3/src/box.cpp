#include "box.hpp"

Box::Box(MaterialType material, glm::mat4 transform, glm::mat4 worldToCamera) 
    : Object(ShapeType::BOX, material, transform, worldToCamera) {}

void Box::setPoints(glm::vec3 minPoint, glm::vec3 maxPoint) {
    mMinPoint = minPoint;
    mMaxPoint = maxPoint;
}

bool Box::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    // Implement ray-box intersection
    // Use mRayTransform for transforming ray before intersection
    // Use mNormalTransfrom for transforming object normal
     // Refer ray-sphere and ray-plane intersection code to get an idea
}

BoxSource::BoxSource(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Box(MaterialType::LIGHT, transform, worldToCamera) {}

void BoxSource::setRadiance(glm::vec3 radiance) {
    mRadiance = radiance;
}

glm::vec3 BoxSource::getRadiance() {
    return mRadiance;
}

DiffuseBox::DiffuseBox(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Box(MaterialType::DIFFUSE, transform, worldToCamera) {}

void DiffuseBox::setAlbedo(glm::vec3 albedo) {
    mAlbedo = albedo;
}

glm::vec3 DiffuseBox::getAlbedo() {
    return mAlbedo;
}

MetallicBox::MetallicBox(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Box(MaterialType::METALLIC, transform, worldToCamera) {}

void MetallicBox::setFresnelConstant(glm::vec3 fresnelConstant) {
    mFresnelConstant = fresnelConstant;
}

glm::vec3 MetallicBox::getFresnelConstant() {
    return mFresnelConstant;
}

glm::vec3 MetallicBox::getFresnelCoefficient(glm::vec3 direction, glm::vec3 normal) {
    direction = - glm::normalize(direction);
    return mFresnelConstant + (1.0f - mFresnelConstant) * powf((1 - glm::dot(direction, normal)), 5);
}

glm::vec3 MetallicBox::getReflectedRayDirection(glm::vec3 direction, glm::vec3 normal) {
    return glm::normalize(direction - 2.0f * glm::dot(direction, normal) * normal);
}

TransparentBox::TransparentBox(glm::mat4 transform, glm::mat4 worldToCamera) 
    : Box(MaterialType::TRANSPARENT, transform, worldToCamera) {}

void TransparentBox::setRefractiveIndex(float refractiveIndex) {
    mRefractiveIndex = refractiveIndex;
}

float TransparentBox::getRefractiveIndex() {
    return mRefractiveIndex;
}

float TransparentBox::getFresnelConstant(float outside, float inside) {
    return powf((outside - inside) / (outside + inside), 2);
}

float TransparentBox::getFresnelCoefficient(float outside, float inside, glm::vec3 direction, glm::vec3 normal) {
    float fresnelConstant = getFresnelConstant(outside, inside);
    direction = - glm::normalize(direction);
    glm::vec3 transmission = getRefractedRayDirection(outside, inside, direction, normal);
    if (glm::dot(direction, normal) < glm::dot(-transmission, normal)) {
        return fresnelConstant + (1.0f - fresnelConstant) * powf(1 - glm::dot(direction, normal), 5);
    } else {
        return fresnelConstant + (1.0f - fresnelConstant) * powf(1 - glm::dot(-transmission, normal), 5);
    }
}

glm::vec3 TransparentBox::getReflectedRayDirection(glm::vec3 direction, glm::vec3 normal) {
    return glm::normalize(direction - 2.0f * glm::dot(direction, normal) * normal);
}

glm::vec3 TransparentBox::getRefractedRayDirection(float outside, float inside, glm::vec3 direction, glm::vec3 normal) {
    float relative = outside / inside;
    return (relative * glm::dot(direction, normal) - sqrtf(1 - powf(relative, 2) * (1 - powf(glm::dot(direction, normal), 2)))) * normal - relative * direction;
}