#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include <glm/glm.hpp>
#include "mesh.hpp"

namespace M = COL781::Mesh;

namespace COL781 {
    namespace RigidBody {
        
        class RigidBody: public M::Mesh {
            protected:
                glm::mat4 mInitTransform;
                glm::vec3 mLinearVelocity;
                float mAngularVelocity;
                glm::vec3 mRotationAxis;
            public:
                RigidBody();
                void update(float);
                void setInitTransform(glm::mat4);
                void setPhysicalParameters(glm::vec3, float, glm::vec3);
                virtual void initialize() = 0;
        };

        class Sphere: public RigidBody {
        };

        class Cylinder: public RigidBody {

        };

        class Rectangle: public RigidBody {
            
        };

    }
}

#endif // RIGID_BODY_HPP