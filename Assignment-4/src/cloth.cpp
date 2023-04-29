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
            mStruct = {100, 20};
            mShear = {50, 10};
            mBend = {10, 2};
            mVelocity.clear();
            mFixed.clear();
            mPBD = false;
        }

        void Cloth::update(float step) {
            float hL = 2 * mLength / mCols;
            float vL = 2 * mWidth / mRows;
            float dL = glm::sqrt(vL * vL + hL * hL);
            if (mPBD) {
                std::vector<glm::vec3> originalPos(mCount);
                for (int i = 0; i < mCount; i ++) {
                    originalPos[i] = mVertices[i].position;
                }
                std::vector<std::vector<glm::vec3>> force(mRows + 1, std::vector<glm::vec3>(mCols + 1, mMass * glm::vec3(0, -9.8, 0)));
                for (int i = 0; i <= mRows; i ++) {
                    for (int j = 0; j <= mCols; j ++) {
                        if (mFixed[i * (mCols + 1) + j]) {
                            continue;
                        }
                        glm::vec3 pos = mVertices[i * (mCols + 1) + j].position, tempPos;
                        glm::vec3 vel = mVelocity[i * (mCols + 1) + j], tempVel;
                        glm::vec3 unit;
                        float len;
                        // force from (i, j + 2)
                        if (j < mCols - 1) {
                            tempPos = mVertices[i * (mCols + 1) + (j + 2)].position;
                            tempVel = mVelocity[i * (mCols + 1) + (j + 2)];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mBend.first * (len - (2 * hL)) + mBend.second * (glm::dot(vel - tempVel, unit))) * unit / (2 * hL);
                        }
                        // force from (i, j - 2)
                        if (j > 1) {
                            tempPos = mVertices[i * (mCols + 1) + (j - 2)].position;
                            tempVel = mVelocity[i * (mCols + 1) + (j - 2)];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mBend.first * (len - (2 * hL)) + mBend.second * (glm::dot(vel - tempVel, unit))) * unit / (2 * hL);
                        }
                        if (i < mRows) {
                            // force from (i + 1, j + 1)
                            if (j < mCols) {
                                tempPos = mVertices[(i + 1) * (mCols + 1) + (j + 1)].position;
                                tempVel = mVelocity[(i + 1) * (mCols + 1) + (j + 1)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mShear.first * (len - dL) + mShear.second * (glm::dot(vel - tempVel, unit))) * unit / dL;
                            }
                            // force from (i + 1, j - 1)
                            if (j > 0) {
                                tempPos = mVertices[(i + 1) * (mCols + 1) + (j - 1)].position;
                                tempVel = mVelocity[(i + 1) * (mCols + 1) + (j - 1)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mShear.first * (len - dL) + mShear.second * (glm::dot(vel - tempVel, unit))) * unit / dL;
                            }
                        }
                        if (i > 0) {
                            // force from (i - 1, j + 1)
                            if (j < mCols) {
                                tempPos = mVertices[(i - 1) * (mCols + 1) + (j + 1)].position;
                                tempVel = mVelocity[(i - 1) * (mCols + 1) + (j + 1)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mShear.first * (len - dL) + mShear.second * (glm::dot(vel - tempVel, unit))) * unit / dL;
                            }
                            // force from (i - 1, j - 1)
                            if (j > 0) {
                                tempPos = mVertices[(i - 1) * (mCols + 1) + (j - 1)].position;
                                tempVel = mVelocity[(i - 1) * (mCols + 1) + (j - 1)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mShear.first * (len - dL) + mShear.second * (glm::dot(vel - tempVel, unit))) * unit / dL;
                            }
                        }
                        // force from (i + 2, j)
                        if (i < mRows - 1) {
                            tempPos = mVertices[(i + 2) * (mCols + 1) + j].position;
                            tempVel = mVelocity[(i + 2) * (mCols + 1) + j];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mBend.first * (len - (2 * vL)) + mBend.second * (glm::dot(vel - tempVel, unit))) * unit / (2 * vL);
                        }
                        // force from (i - 2, j)
                        if (i > 1) {
                            tempPos = mVertices[(i - 2) * (mCols + 1) + j].position;
                            tempVel = mVelocity[(i - 2) * (mCols + 1) + j];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mBend.first * (len - (2 * vL)) + mBend.second * (glm::dot(vel - tempVel, unit))) * unit / (2 * vL);
                        }
                    }
                }
                for (int i = 0; i <= mRows; i ++) {
                    for (int j = 0; j <= mCols; j ++) {
                        if (mFixed[i * (mCols + 1) + j]) {
                            continue;
                        }
                        mVelocity[i * (mCols + 1) + j] += step * force[i][j] / mMass;
                        mVertices[i * (mCols + 1) + j].position += step * mVelocity[i * (mCols + 1) + j];
                    }
                }
                // constraint projections
                for (int k = 0; k < mNumIter; k ++) {
                    for (int i = 0; i <= mRows; i ++) {
                        for (int j = 0; j <= mCols; j ++) {
                            if (mFixed[i * (mCols + 1) + j]) {
                                continue;
                            }
                            glm::vec3 pos, tempPos;
                            glm::vec3 unit;
                            float len;
                            // project (i, j + 1) constraint
                            if (j < mCols) {
                                pos = mVertices[i * (mCols + 1) + j].position;
                                tempPos = mVertices[i * (mCols + 1) + (j + 1)].position;
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                if (mFixed[i * (mCols + 1) + (j + 1)]) {
                                    pos += (hL - len) * unit;
                                    mVertices[i * (mCols + 1) + j].position = pos;
                                } else {
                                    pos += 0.5f * (hL - len) * unit;
                                    tempPos += 0.5f * (len - hL) * unit;
                                    mVertices[i * (mCols + 1) + j].position = pos;
                                    mVertices[i * (mCols + 1) + (j + 1)].position = tempPos;
                                }
                            }
                            // project (i, j - 1) constraint
                            if (j > 0) {
                                pos = mVertices[i * (mCols + 1) + j].position;
                                tempPos = mVertices[i * (mCols + 1) + (j - 1)].position;
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                if (mFixed[i * (mCols + 1) + (j - 1)]) {
                                    pos += (hL - len) * unit;
                                    mVertices[i * (mCols + 1) + j].position = pos;
                                } else {
                                    pos += 0.5f * (hL - len) * unit;
                                    tempPos += 0.5f * (len - hL) * unit;
                                    mVertices[i * (mCols + 1) + j].position = pos;
                                    mVertices[i * (mCols + 1) + (j - 1)].position = tempPos;
                                }
                            }
                            // project (i + 1, j) constraint
                            if (i < mRows) {
                                pos = mVertices[i * (mCols + 1) + j].position;
                                tempPos = mVertices[(i + 1) * (mCols + 1) + j].position;
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                if (mFixed[(i + 1) * (mCols + 1) + j]) {
                                    pos += (vL - len) * unit;
                                    mVertices[i * (mCols + 1) + j].position = pos;
                                } else {
                                    pos += 0.5f * (vL - len) * unit;
                                    tempPos += 0.5f * (len - vL) * unit;
                                    mVertices[i * (mCols + 1) + j].position = pos;
                                    mVertices[(i + 1) * (mCols + 1) + j].position = tempPos;
                                }
                            }
                            // project (i - 1, j) constraint
                            if (i > 0) {
                                pos = mVertices[i * (mCols + 1) + j].position;
                                tempPos = mVertices[(i - 1) * (mCols + 1) + j].position;
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                if (mFixed[(i - 1) * (mCols + 1) + j]) {
                                    pos += (vL - len) * unit;
                                    mVertices[i * (mCols + 1) + j].position = pos;
                                } else {
                                    pos += 0.5f * (vL - len) * unit;
                                    tempPos += 0.5f * (len - vL) * unit;
                                    mVertices[i * (mCols + 1) + j].position = pos;
                                    mVertices[(i - 1) * (mCols + 1) + j].position = tempPos;
                                }
                            }
                        }
                    }
                }
                for (int i = 0; i <= mRows; i ++) {
                    for (int j = 0; j <= mCols; j ++) {
                        if (mFixed[i * (mCols + 1) + j]) {
                            continue;
                        }
                        mVelocity[i * (mCols + 1) + j] = (mVertices[i * (mCols + 1) + j].position - originalPos[i * (mCols + 1) + j]) / step;
                    }
                }
            } else {
                std::vector<std::vector<glm::vec3>> force(mRows + 1, std::vector<glm::vec3>(mCols + 1, mMass * glm::vec3(0, -9.8, 0)));
                for (int i = 0; i <= mRows; i ++) {
                    for (int j = 0; j <= mCols; j ++) {
                        if (mFixed[i * (mCols + 1) + j]) {
                            continue;
                        }
                        glm::vec3 pos = mVertices[i * (mCols + 1) + j].position, tempPos;
                        glm::vec3 vel = mVelocity[i * (mCols + 1) + j], tempVel;
                        glm::vec3 unit;
                        float len;
                        // force from (i, j + 1)
                        if (j < mCols) {
                            tempPos = mVertices[i * (mCols + 1) + (j + 1)].position;
                            tempVel = mVelocity[i * (mCols + 1) + (j + 1)];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mStruct.first * (len - hL) + mStruct.second * (glm::dot(vel - tempVel, unit))) * unit / hL;
                            // force from (i, j + 2)
                            if (j < mCols - 1) {
                                tempPos = mVertices[i * (mCols + 1) + (j + 2)].position;
                                tempVel = mVelocity[i * (mCols + 1) + (j + 2)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mBend.first * (len - (2 * hL)) + mBend.second * (glm::dot(vel - tempVel, unit))) * unit / (2 * hL);
                            }
                        }
                        // force from (i, j - 1)
                        if (j > 0) {
                            tempPos = mVertices[i * (mCols + 1) + (j - 1)].position;
                            tempVel = mVelocity[i * (mCols + 1) + (j - 1)];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mStruct.first * (len - hL) + mStruct.second * (glm::dot(vel - tempVel, unit))) * unit / hL;
                            // force from (i, j - 2)
                            if (j > 1) {
                                tempPos = mVertices[i * (mCols + 1) + (j - 2)].position;
                                tempVel = mVelocity[i * (mCols + 1) + (j - 2)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mBend.first * (len - (2 * hL)) + mBend.second * (glm::dot(vel - tempVel, unit))) * unit / (2 * hL);
                            }
                        }
                        // force from (i + 1, j)
                        if (i < mRows) {
                            tempPos = mVertices[(i + 1) * (mCols + 1) + j].position;
                            tempVel = mVelocity[(i + 1) * (mCols + 1) + j];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mStruct.first * (len - vL) + mStruct.second * (glm::dot(vel - tempVel, unit))) * unit / vL;
                            // force from (i + 1, j + 1)
                            if (j < mCols) {
                                tempPos = mVertices[(i + 1) * (mCols + 1) + (j + 1)].position;
                                tempVel = mVelocity[(i + 1) * (mCols + 1) + (j + 1)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mShear.first * (len - dL) + mShear.second * (glm::dot(vel - tempVel, unit))) * unit / dL;
                            }
                            // force from (i + 1, j - 1)
                            if (j > 0) {
                                tempPos = mVertices[(i + 1) * (mCols + 1) + (j - 1)].position;
                                tempVel = mVelocity[(i + 1) * (mCols + 1) + (j - 1)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mShear.first * (len - dL) + mShear.second * (glm::dot(vel - tempVel, unit))) * unit / dL;
                            }
                        }
                        // force from (i - 1, j)
                        if (i > 0) {
                            tempPos = mVertices[(i - 1) * (mCols + 1) + j].position;
                            tempVel = mVelocity[(i - 1) * (mCols + 1) + j];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mStruct.first * (len - vL) + mStruct.second * (glm::dot(vel - tempVel, unit))) * unit / vL;
                            // force from (i - 1, j + 1)
                            if (j < mCols) {
                                tempPos = mVertices[(i - 1) * (mCols + 1) + (j + 1)].position;
                                tempVel = mVelocity[(i - 1) * (mCols + 1) + (j + 1)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mShear.first * (len - dL) + mShear.second * (glm::dot(vel - tempVel, unit))) * unit / dL;
                            }
                            // force from (i - 1, j - 1)
                            if (j > 0) {
                                tempPos = mVertices[(i - 1) * (mCols + 1) + (j - 1)].position;
                                tempVel = mVelocity[(i - 1) * (mCols + 1) + (j - 1)];
                                len = glm::length(pos - tempPos);
                                unit = (pos - tempPos) / len;
                                force[i][j] -= (mShear.first * (len - dL) + mShear.second * (glm::dot(vel - tempVel, unit))) * unit / dL;
                            }
                        }
                        // force from (i + 2, j)
                        if (i < mRows - 1) {
                            tempPos = mVertices[(i + 2) * (mCols + 1) + j].position;
                            tempVel = mVelocity[(i + 2) * (mCols + 1) + j];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mBend.first * (len - (2 * vL)) + mBend.second * (glm::dot(vel - tempVel, unit))) * unit / (2 * vL);
                        }
                        // force from (i - 2, j)
                        if (i > 1) {
                            tempPos = mVertices[(i - 2) * (mCols + 1) + j].position;
                            tempVel = mVelocity[(i - 2) * (mCols + 1) + j];
                            len = glm::length(pos - tempPos);
                            unit = (pos - tempPos) / len;
                            force[i][j] -= (mBend.first * (len - (2 * vL)) + mBend.second * (glm::dot(vel - tempVel, unit))) * unit / (2 * vL);
                        }   
                    }
                }
                for (int i = 0; i <= mRows; i ++) {
                    for (int j = 0; j <= mCols; j ++) {
                        if (mFixed[i * (mCols + 1) + j]) {
                            continue;
                        }
                        mVelocity[i * (mCols + 1) + j] += step * force[i][j] / mMass;
                        mVertices[i * (mCols + 1) + j].position += step * mVelocity[i * (mCols + 1) + j];
                    }
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

        void Cloth::setPhysicalParameters(float mass, float radius, std::pair<float, float> kStruct, std::pair<float, float> kShear, std::pair<float, float> kBend) {
            mMass = mass;
            mRadius = radius;
            mStruct = kStruct;
            mShear = kShear;
            mBend = kBend;
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