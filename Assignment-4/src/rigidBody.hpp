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
                glm::mat4 mConstantTransform;
            public:
                RigidBody();
                void update(float);
                void setInitTransform(glm::mat4);
                void setConstantTransform(glm::mat4);
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