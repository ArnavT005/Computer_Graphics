#include "box.hpp"
#include "plane.hpp"

Box::Box() {}

void Box::setMinPoint(glm::vec3 point) {
    mMinPoint = point;
}

void Box::setMaxPoint(glm::vec3 point) {
    mMaxPoint = point;
}

bool Box::intersectRay(glm::vec3 origin, glm::vec3 direction, float tMin, float tMax) {
    Plane planes[6];
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