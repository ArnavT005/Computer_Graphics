#include "cloth.hpp"

namespace COL781 {
    namespace Cloth {
        Cloth::Cloth() {
            mInitTransform = glm::mat4(1.0f);
            mRows = 1;
            mCols = 1;
            mLength = 0.5f;
            mWidth = 0.5f;
            mPBD = false;
        }

        void Cloth::update(float delta_t) {

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

        void Cloth::setPhysicalParameters(float param) {

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
        }

    }
}