#include "rigidBody.hpp"
#include "mesh.hpp"


namespace COL781 {
    namespace RigidBody {
        RigidBody::RigidBody() {
            mInitTransform = glm::mat4(1.0f);
            mConstantTransform = glm::mat4(1.0f);
        }

        void RigidBody::update(float delta_t) {
            // get vertices and normals
            std::vector<glm::vec3> vertices = getVertices();
            std::vector<glm::vec3> normals = getNormals();
            // transform vertices and normals
            for (int i = 0; i < vertices.size(); i ++) {
                vertices[i] = glm::vec3(mConstantTransform * glm::vec4(vertices[i], 1.0f));
                normals[i] = glm::vec3(glm::inverse(glm::transpose(mConstantTransform)) * glm::vec4(normals[i], 0.0f));
            }
            // destroy and set vertices and normals
            destroy();
            setVertices(vertices.size(), vertices.data(), normals.data());
            // connect
            connect();
        }   

        void RigidBody::setInitTransform(glm::mat4 initTransform) {
            mInitTransform = initTransform;
        }

        void RigidBody::setConstantTransform(glm::mat4 constantTransform) {
            mConstantTransform = constantTransform;
        }


    }
}