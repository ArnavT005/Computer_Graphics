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

        Sphere::Sphere() {
            mRadius = 0.5f;
            mLongitude = mLatitude = 8;
        }

        void Sphere::setGeometricParameters(int longitude, int latitude, float radius) {
            mLongitude = longitude;
            mLatitude = latitude;
            mRadius = radius;
        }

        void Sphere::initialize() {
            createSphereMesh(mLongitude, mLatitude, mRadius);
            for (int i = 0; i < mVertices.size(); i ++) {
                mVertices[i].position = glm::vec3(mInitTransform * glm::vec4(mVertices[i].position, 1.0f));
                mVertices[i].normal = glm::vec3(glm::inverse(glm::transpose(mInitTransform)) * glm::vec4(mVertices[i].normal, 0.0f));
            }
            for (int i = 0; i < mFaces.size() - mVirtualFaces.size(); i ++) {
                glm::vec3 a = mVertices[mFaces[i].indices[1]].position - mVertices[mFaces[i].indices[0]].position;
                glm::vec3 b = mVertices[mFaces[i].indices[2]].position - mVertices[mFaces[i].indices[0]].position;
                mFaces[i].normal = glm::normalize(glm::cross(a, b));
            }
        }

        Rectangle::Rectangle() {
            mRows = 1;
            mCols = 1;
            mLength = 0.5f;
            mWidth = 0.5f;
        }

        void Rectangle::setGeometricParameters(int rows, int cols, float length, float width) {
            mRows = rows;
            mCols = cols;
            mLength = length;
            mWidth = width;
        }

        void Rectangle::initialize() {
            createRectangleMesh(mRows, mCols, mLength, mWidth);
            for (int i = 0; i < mVertices.size(); i ++) {
                mVertices[i].position = glm::vec3(mInitTransform * glm::vec4(mVertices[i].position, 1.0f));
                mVertices[i].normal = glm::vec3(glm::inverse(glm::transpose(mInitTransform)) * glm::vec4(mVertices[i].normal, 0.0f));
            }
            for (int i = 0; i < mFaces.size() - mVirtualFaces.size(); i ++) {
                glm::vec3 a = mVertices[mFaces[i].indices[1]].position - mVertices[mFaces[i].indices[0]].position;
                glm::vec3 b = mVertices[mFaces[i].indices[2]].position - mVertices[mFaces[i].indices[0]].position;
                mFaces[i].normal = glm::normalize(glm::cross(a, b));
            }
        }

    }
}