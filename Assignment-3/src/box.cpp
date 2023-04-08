#include "box.hpp"
#include "plane.hpp"

Box::Box(MaterialType material, glm::mat4 transform, glm::mat4 worldToCamera) 
    : Object(ShapeType::BOX, material, transform, worldToCamera) {}

void Box::setPoints(glm::vec3 minPoint, glm::vec3 maxPoint) {
    mMinPoint = minPoint;
    mMaxPoint = maxPoint;
}

bool Box::isInside(glm::vec3 point) {
    // return true if point is inside the box, false otherwise
}

bool Box::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    // Implement ray-box intersection
    // Use mRayTransform for transforming ray before intersection
    // Use mNormalTransfrom for transforming object normal
    // Refer ray-sphere and ray-plane intersection code to get an idea
    DiffusePlane planes[6];
    planes[0].setPoint(glm::vec3(mMinPoint.x, mMinPoint.y, mMinPoint.z));
    planes[0].setNormal(glm::vec3(0.0f, 0.0f, -1.0f));
    planes[1].setPoint(glm::vec3(mMinPoint.x, mMinPoint.y, mMaxPoint.z));
    planes[1].setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    planes[2].setPoint(glm::vec3(mMinPoint.x, mMinPoint.y, mMinPoint.z));
    planes[2].setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
    planes[3].setPoint(glm::vec3(mMinPoint.x, mMaxPoint.y, mMinPoint.z));
    planes[3].setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    planes[4].setPoint(glm::vec3(mMinPoint.x, mMinPoint.y, mMinPoint.z));
    planes[4].setNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
    planes[5].setPoint(glm::vec3(mMaxPoint.x, mMinPoint.y, mMinPoint.z));
    planes[5].setNormal(glm::vec3(1.0f, 0.0f, 0.0f));
    for (int i = 0; i < 6; i++) {
        if (planes[i].intersectRay(origin, direction, tMin, tMax)) {
            return true;
        }
    }
    return false;
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

void MetallicBox::setAlbedo(glm::vec3 albedo) {
    mAlbedo = albedo;
}

void MetallicBox::setFresnelConstant(glm::vec3 fresnelConstant) {
    mFresnelConstant = fresnelConstant;
}

glm::vec3 MetallicBox::getAlbedo() {
    return mAlbedo;
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

void TransparentBox::setInternalRefractiveIndex(float internalRefractiveIndex) {
    mInternalRefractiveIndex = internalRefractiveIndex;
}

void TransparentBox::setExternalRefractiveIndex(float externalRefractiveIndex) {
    mExternalRefractiveIndex = externalRefractiveIndex;
}

float TransparentBox::getInternalRefractiveIndex() {
    return mInternalRefractiveIndex;
}

float TransparentBox::getExternalRefractiveIndex() {
    return mExternalRefractiveIndex;
}

float TransparentBox::getFresnelConstant(float outside, float inside) {
    return powf((outside - inside) / (outside + inside), 2);
}

float TransparentBox::getFresnelCoefficient(float outside, float inside, glm::vec3 direction, glm::vec3 normal) {
    float fresnelConstant = getFresnelConstant(outside, inside);
    glm::vec3 transmission = getRefractedRayDirection(outside, inside, direction, normal);
    direction = - glm::normalize(direction);
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
    direction = - glm::normalize(direction);
    if (1 - powf(relative, 2) * (1 - powf(glm::dot(direction, normal), 2)) < 0) {
        return glm::vec3(0);
    }
    return glm::normalize((relative * glm::dot(direction, normal) - sqrtf(1 - powf(relative, 2) * (1 - powf(glm::dot(direction, normal), 2)))) * normal - relative * direction);
}