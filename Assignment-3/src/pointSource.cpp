#include "pointSource.hpp"

PointSource::PointSource() {}

void PointSource::setCoordinate(glm::vec3 coordinate) {
    mCoordinate = coordinate;
}

void PointSource::setIntensity(glm::vec3 intensity) {
    mIntensity = intensity;
}

void PointSource::setWorldToCamera(glm::mat4 worldToCamera) {
    mWorldToCamera = worldToCamera;
}

glm::vec3 PointSource::getIrradiance(glm::vec3 point, glm::vec3 normal) {
    glm::vec3 cameraCoordinate(mWorldToCamera * glm::vec4(mCoordinate, 1.0f));
    glm::vec3 lightDirection = glm::normalize(cameraCoordinate - point);
    float squaredDistance = glm::dot(cameraCoordinate - point, cameraCoordinate - point);
    return (mIntensity * glm::dot(normal, lightDirection)) / squaredDistance;
}