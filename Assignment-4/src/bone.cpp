#include "bone.hpp"

namespace COL781 {
    namespace Bone {
        Bone::Bone() {
            pMParent = nullptr;
            mPosition = glm::vec3(0);
            mRotationAxis = glm::vec3(0);
            mJointAngle = 0.0f;
            pMCollider = nullptr;
            mChildren.clear();
        }

        void Bone::setParent(Bone *pParent, glm::vec3 position, glm::vec3 rotationAxis, float jointAngle) {
            pMParent = pParent;
            mPosition = position;
            mRotationAxis = rotationAxis;
            mJointAngle = jointAngle;
        }

        void Bone::setCollider(R::RigidBody *pCollider) {
            pMCollider = pCollider;
        }

        void Bone::addChild(Bone *pChild) {
            mChildren.push_back(pChild);
        }

        void Bone::update(float jointAngle) {
            mJointAngle = jointAngle;
        }

        glm::mat4 Bone::getTransform() {
            return glm::rotate(glm::translate(glm::mat4(1.0f), mPosition), mJointAngle, mRotationAxis);
        }

    }
}