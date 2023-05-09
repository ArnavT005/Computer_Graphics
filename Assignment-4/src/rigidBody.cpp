#include "rigidBody.hpp"
#include "mesh.hpp"


namespace COL781 {
    namespace RigidBody {
        RigidBody::RigidBody(M::MeshType type) : Mesh(type) {
            mInitTransform = glm::mat4(1.0f);
            mLinearVelocity = glm::vec3(0);
            mAngularVelocity = 0;
            mRotationAxis = glm::vec3(0);
            mRotationPoint = glm::vec3(0);
            mRestitution = 0;
            mFriction = 0;
        }

        void RigidBody::update(float step) {
            glm::mat4 transform = glm::mat4(1.0f);
            if (mAngularVelocity > 1e-6) {
                transform = glm::translate(glm::rotate(glm::translate(glm::mat4(1.0f), mRotationPoint), mAngularVelocity * step, mRotationAxis), - mRotationPoint);
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

        void RigidBody::setPhysicalParameters(glm::vec3 linearVelocity, float angularVelocity, glm::vec3 rotationAxis, glm::vec3 rotationPoint) {
            mLinearVelocity = linearVelocity;
            mAngularVelocity = angularVelocity;
            mRotationAxis = rotationAxis;
            mRotationPoint = rotationPoint;
        }

        void RigidBody::setCollisionParameters(float restitution, float friction) {
            mRestitution = restitution;
            mFriction = friction;
        }

        Sphere::Sphere() : RigidBody(M::MeshType::RIGID_SPHERE) {
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

        void Sphere::checkCollision(glm::vec3 &pos, glm::vec3 &vel, float mass, float radius) {
            glm::vec3 center = (mVertices.back().position + mVertices[0].position) / 2.0f;
            glm::vec3 n = glm::normalize(pos - center);
            float dist = glm::length(pos - center) - (mRadius + radius);
            if (dist >= 0) {
                return;
            }
            if (glm::dot(vel, n) < 0) {
                glm::vec3 nI = - (1.0f + mRestitution) * mass * glm::dot(vel, n) * n;
                glm::vec3 tV = vel - glm::dot(vel, n) * n;
                glm::vec3 tI = - glm::min(mFriction * glm::length(nI), mass * glm::length(tV)) * tV / glm::length(tV);
                vel += (nI + tI) / mass;
            }
            pos -= dist * n;
        }

        Rectangle::Rectangle() : RigidBody(M::MeshType::RIGID_RECTANGLE) {
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

        void Rectangle::checkCollision(glm::vec3 &pos, glm::vec3 &vel, float mass, float radius) {
            glm::vec3 a = mVertices[mCols + 1].position - mVertices[0].position;
            glm::vec3 b = mVertices[1].position - mVertices[0].position;
            glm::vec3 n = glm::normalize(glm::cross(a, b));
            float dist = glm::dot(pos - mVertices[0].position, n) - radius;
            if (dist >= 0) {
                return;
            }
            if (glm::dot(vel, n) < 0) {
                glm::vec3 nI = - (1.0f + mRestitution) * mass * glm::dot(vel, n) * n;
                glm::vec3 tV = vel - glm::dot(vel, n) * n;
                glm::vec3 tI = - glm::min(mFriction * glm::length(nI), mass * glm::length(tV)) * tV / glm::length(tV);
                vel += (nI + tI) / mass;
            }
            pos -= dist * n;
        }

        Cylinder::Cylinder() : RigidBody(M::MeshType::RIGID_CYLINDER) {
            mNumSides = 4;
            mLength = 0.5f;
            mRadius = 0.25f;
        }

        void Cylinder::setGeometricParameters(int numSides, float length, float radius) {
            mNumSides = numSides;
            mLength = length;
            mRadius = radius;
        }

        void Cylinder::initialize() {
            createCylinderMesh(mNumSides, mLength, mRadius);
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

        void Cylinder::checkCollision(glm::vec3 &pos, glm::vec3 &vel, float mass, float radius) {
            int numVertices = mVertices.size();
            glm::vec3 n1 = glm::normalize(mVertices[numVertices - 2].position - mVertices[numVertices - 1].position), n2 = - n1;
            float dist1 = glm::dot(n1, pos - mVertices[numVertices - 2].position) - radius, dist2 = glm::dot(n2, pos - mVertices[numVertices - 1].position) - radius;
            if (dist1 >= 0 || dist2 >= 0) {
                return;
            }
            glm::vec3 normalComponent = (pos - mVertices[numVertices - 1].position) - glm::dot(n1, (pos - mVertices[numVertices - 1].position)) * n1;
            float dist3 = glm::length(normalComponent) - (mRadius + radius);
            glm::vec3 n3 = glm::normalize(normalComponent);
            if (dist3 >= 0) {
                return;
            }
            glm::vec3 n;
            float dist;
            if (dist1 >= dist2 && dist1 >= dist3) {
                n = n1;
                dist = dist1;
            } else if (dist2 >= dist1 && dist2 >= dist3) {
                n = n2;
                dist = dist2;
            } else {
                n = n3;
                dist = dist3;
            }
            if (glm::dot(vel, n) < 0) {
                glm::vec3 nI = - (1.0f + mRestitution) * mass * glm::dot(vel, n) * n;
                glm::vec3 tV = vel - glm::dot(vel, n) * n;
                glm::vec3 tI = glm::length(tV) == 0 ? glm::vec3(0) : - glm::min(mFriction * glm::length(nI), mass * glm::length(tV)) * tV / glm::length(tV);
                vel += (nI + tI) / mass;
            }
            pos -= dist * n;
        }

    }
}