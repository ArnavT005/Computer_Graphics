#include "bone.hpp"

namespace COL781 {
    namespace Bone {
        glm::mat4 Bone::getTransform() {
            return glm::rotate(glm::translate(glm::mat4(1.0f), mPosition), mJointAngle, mRotationAxis);
        }

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

        void Bone::update(glm::mat4 &parentTransform) {
            glm::mat4 myTransform = parentTransform * getTransform();
            pMCollider->update(myTransform);
            for (Bone *child : mChildren) {
                child->update(myTransform);
            }
        }

        void Bone::setAnimationControls(std::vector<float> &controls, int &index) {
            mJointAngle = controls[index ++];
            for (Bone *child : mChildren) {
                child->setAnimationControls(controls, index);
            }
        }

        std::vector<float> Bone::getAnimationControls() {
            std::vector<float> controls(1, mJointAngle);
            for (Bone *child : mChildren) {
                std::vector<float> childControls = child->getAnimationControls();
                controls.insert(controls.end(), childControls.begin(), childControls.end());
            }
            return controls;
        }

        std::vector<M::Mesh*> Bone::getRigidBodies() {
            std::vector<M::Mesh*> rigidBodies(1, pMCollider);
            for (Bone *child : mChildren) {
                std::vector<M::Mesh*> childRigidBodies = child->getRigidBodies();
                rigidBodies.insert(rigidBodies.end(), childRigidBodies.begin(), childRigidBodies.end());
            }
            return rigidBodies;
        }

    }
}