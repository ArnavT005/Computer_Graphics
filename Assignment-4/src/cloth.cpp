#include "cloth.hpp"

namespace COL781 {
    namespace Cloth {
        Cloth::Cloth() {
            mInitTransform = glm::mat4(1.0f);
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
            
        }

        void Cloth::setInitTransform(glm::mat4 initTransform) {
            mInitTransform = initTransform;
        }

        void Cloth::setGeometricParameters(int rows, int cols, float length, float width) {
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

        void Cloth::enablePBD() {
            mPBD = true;
        }

        void Cloth::initialize() {
            createRectangleMesh(mRows, mCols, mLength, mWidth);
            std::vector<glm::vec3> vertices = getVertices();
            std::vector<glm::vec3> normals = getNormals();
            std::vector<glm::ivec3> faces = getFaces();
            for (int i = 0; i < vertices.size(); i ++) {
                vertices[i] = glm::vec3(mInitTransform * glm::vec4(vertices[i], 1.0f));
                normals[i] = glm::vec3(glm::inverse(glm::transpose(mInitTransform)) * glm::vec4(normals[i], 0.0f));
            }
            destroy();
            setVertices(vertices.size(), vertices.data(), normals.data());
            setFaces(faces.size(), faces.data());
            connect();
            mVelocity.resize(vertices.size(), glm::vec3(0));
        }

    }
}