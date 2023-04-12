#include "box.hpp"

Box::Box(MaterialType material, glm::mat4 transform, glm::mat4 worldToCamera) 
    : Object(ShapeType::BOX, material, transform, worldToCamera) {}

void Box::setPoints(glm::vec3 minPoint, glm::vec3 maxPoint) {
    mMinPoint = minPoint;
    mMaxPoint = maxPoint;
}

bool Box::isInside(glm::vec3 point) {
    float bias = 0.000001;
    return (point.x >= (mMinPoint.x - bias) && point.x <= (mMaxPoint.x + bias) && point.y >= (mMinPoint.y - bias) && point.y <= (mMaxPoint.y + bias) && point.z >= (mMinPoint.z - bias) && point.z <= (mMaxPoint.z + bias));
}

bool Box::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    glm::vec3 transformedOrigin(mRayTransform * glm::vec4(origin, 1.0f));
    glm::vec3 transformedDirection(mRayTransform * glm::vec4(direction, 0.0f));
    float normalSign = isInside(transformedOrigin + 0.001f * transformedDirection) ? -1 : 1;
    float minT = tMin, maxT = tMax;
    if (glm::abs(transformedDirection.x) > 0.000001) {
        if (transformedDirection.x > 0) {
            minT = glm::max(minT, (mMinPoint.x - transformedOrigin.x) / transformedDirection.x);
            maxT = glm::min(maxT, (mMaxPoint.x - transformedOrigin.x) / transformedDirection.x);
        } else {
            minT = glm::max(minT, (mMaxPoint.x - transformedOrigin.x) / transformedDirection.x);
            maxT = glm::min(maxT, (mMinPoint.x - transformedOrigin.x) / transformedDirection.x);
        }
    } else {
        if ((transformedOrigin.x < (mMinPoint.x - 0.000001)) || (transformedOrigin.x > (mMaxPoint.x + 0.000001))) {
            return false;
        }
    }
    if (glm::abs(transformedDirection.y) > 0.000001) {
        if (transformedDirection.y > 0) {
            minT = glm::max(minT, (mMinPoint.y - transformedOrigin.y) / transformedDirection.y);
            maxT = glm::min(maxT, (mMaxPoint.y - transformedOrigin.y) / transformedDirection.y);
        } else {
            minT = glm::max(minT, (mMaxPoint.y - transformedOrigin.y) / transformedDirection.y);
            maxT = glm::min(maxT, (mMinPoint.y - transformedOrigin.y) / transformedDirection.y);
        }
    } else {
        if ((transformedOrigin.y < (mMinPoint.y - 0.000001)) || (transformedOrigin.y > (mMaxPoint.y + 0.000001))) {
            return false;
        }
    }
    if (glm::abs(transformedDirection.z) > 0.000001) {
        if (transformedDirection.z > 0) {
            minT = glm::max(minT, (mMinPoint.z - transformedOrigin.z) / transformedDirection.z);
            maxT = glm::min(maxT, (mMaxPoint.z - transformedOrigin.z) / transformedDirection.z);
        } else {
            minT = glm::max(minT, (mMaxPoint.z - transformedOrigin.z) / transformedDirection.z);
            maxT = glm::min(maxT, (mMinPoint.z - transformedOrigin.z) / transformedDirection.z);
        }
    } else {
        if ((transformedOrigin.z < (mMinPoint.z - 0.000001)) || (transformedOrigin.z > (mMaxPoint.z + 0.000001))) {
            return false;
        }
    }
    if (minT > maxT + 0.000001) {
        return false;
    }
    mTValue = minT;
    mIntersectionPoint = origin + mTValue * direction;
    glm::vec3 intersectionPoint = transformedOrigin + mTValue * transformedDirection;
    if (glm::abs(intersectionPoint.x - mMinPoint.x) < 0.000001) {
        mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(-1, 0, 0, 0)));
    } else if (glm::abs(intersectionPoint.x - mMaxPoint.x) < 0.000001) {
        mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(1, 0, 0, 0)));
    } else if (glm::abs(intersectionPoint.y - mMinPoint.y) < 0.000001) {
        mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(0, -1, 0, 0)));
    } else if (glm::abs(intersectionPoint.y - mMaxPoint.y) < 0.000001) {
        mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(0, 1, 0, 0)));
    } else if (glm::abs(intersectionPoint.z - mMinPoint.z) < 0.000001) {
        mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(0, 0, -1, 0)));
    } else {
        mIntersectionNormal = normalSign * glm::normalize(glm::vec3(mNormalTransform * glm::vec4(0, 0, 1, 0)));
    }
    return true;
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