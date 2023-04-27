#include "cloth.hpp"

namespace COL781 {
    namespace Cloth {
        Cloth::Cloth() {
            mInitTransform = glm::mat4(1.0f);
            mCount = 4;
            mRows = 1;
            mCols = 1;
            mLength = 0.5f;
            mWidth = 0.5f;
            mMass = 1.0f;
            mRadius = 0.05;
            mStruct = 1;
            mShear = 0.5;
            mBend = 0.1;
            mDamp = 0.05;
            mVelocity.clear();
            mFixed.clear();
            mPBD = false;
        }

        void Cloth::update(float step) {
            if (mPBD) {

            } else {
                std::vector<glm::vec3> force(mCount, glm::vec3(0));
                for (int i = 0; i <= mRows; i ++) {
                    for (int j = 0; j <= mCols; j ++) {
                        if (mFixed[i * (mCols + 1) + j]) {
                            continue;
                        }
                        // calculate force on each particle
                        force[i * (mCols + 1) + j] = glm::vec3(0, -0.005, 0);
                    }
                }
                for (int i = 0; i < mCount; i ++) {
                    mVelocity[i] += step * force[i] / mMass;
                    mVertices[i].position += step * mVelocity[i];
                }
            }
            for (int i = 0; i < mFaces.size() - mVirtualFaces.size(); i ++) {
                glm::vec3 a = mVertices[mFaces[i].indices[1]].position - mVertices[mFaces[i].indices[0]].position;
                glm::vec3 b = mVertices[mFaces[i].indices[2]].position - mVertices[mFaces[i].indices[0]].position;
                mFaces[i].normal = glm::normalize(glm::cross(a, b));
            }
            computeAndSetVertexNormals();
        }

        void Cloth::setInitTransform(glm::mat4 initTransform) {
            mInitTransform = initTransform;
        }

        void Cloth::setGeometricParameters(int rows, int cols, float length, float width) {
            mCount = (rows + 1) * (cols + 1);
            mRows = rows;
            mCols = cols;
            mLength = length;
            mWidth = width;
        }

        void Cloth::setPhysicalParameters(float mass, float radius, float kStruct, float kShear, float kBend, float kDamp) {
            mMass = mass;
            mRadius = radius;
            mStruct = kStruct;
            mShear = kShear;
            mBend = kBend;
            mDamp = kDamp;          
        }

        void Cloth::setFixedPoints(std::vector<bool> &fixed) {
            mFixed.resize(fixed.size());
            for (int i = 0; i < fixed.size(); i ++) {
                mFixed[i] = fixed[i];
            }
        }

        void Cloth::enablePBD(int numIter) {
            mPBD = true;
            mNumIter = numIter;
        }

        void Cloth::initialize() {
            createRectangleMesh(mRows, mCols, mLength, mWidth);
            for (int i = 0; i < mCount; i ++) {
                mVertices[i].position = glm::vec3(mInitTransform * glm::vec4(mVertices[i].position, 1.0f));
                mVertices[i].normal = glm::vec3(glm::inverse(glm::transpose(mInitTransform)) * glm::vec4(mVertices[i].normal, 0.0f));
            }
            for (int i = 0; i < mFaces.size() - mVirtualFaces.size(); i ++) {
                glm::vec3 a = mVertices[mFaces[i].indices[1]].position - mVertices[mFaces[i].indices[0]].position;
                glm::vec3 b = mVertices[mFaces[i].indices[2]].position - mVertices[mFaces[i].indices[0]].position;
                mFaces[i].normal = glm::normalize(glm::cross(a, b));
            }
            mVelocity.resize(mCount, glm::vec3(0));
        }

    }
}