#include "rigidBody.hpp"
#include "mesh.hpp"


namespace COL781 {
    namespace RigidBody {
        RigidBody::RigidBody() {
            mInitTransform = glm::mat4(1.0f);
            mLinearVelocity = glm::vec3(0);
            mAngularVelocity = 0;
            mRotationAxis = glm::vec3(0);
        }

        void RigidBody::update(float step) {
            glm::mat4 transform = glm::mat4(1.0f);
            if (mAngularVelocity > 1e-6) {
                transform = glm::rotate(glm::mat4(1.0f), mAngularVelocity * step, mRotationAxis);
            } else {
                transform = glm::translate(glm::mat4(1.0f), mLinearVelocity * step);
            }
            for (int i = 0; i < mVertices.size(); i ++) {
                mVertices[i].position = glm::vec3(transform * glm::vec4(mVertices[i].position, 1));
                mVertices[i].normal = glm::vec3(glm::inverse(glm::transpose(transform)) * glm::vec4(mVertices[i].normal, 0));
            }
            for (int i = 0; i < mFaces.size() - mVirtualFaces.size(); i ++) {
                glm::vec3 a = mVertices[mFaces[i].indices[1]].position - mVertices[mFaces[i].indices[0]].position;
                glm::vec3 b = mVertices[mFaces[i].indices[2]].position - mVertices[mFaces[i].indices[0]].position;
                mFaces[i].normal = glm::normalize(glm::cross(a, b));
            }
        }   

        void RigidBody::setInitTransform(glm::mat4 initTransform) {
            mInitTransform = initTransform;
        }

        void RigidBody::setPhysicalParameters(glm::vec3 linearVelocity, float angularVelocity, glm::vec3 rotationAxis) {
            mLinearVelocity = linearVelocity;
            mAngularVelocity = angularVelocity;
            mRotationAxis = rotationAxis;
        }

    }
}