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
            private:
                float mRadius;
                int mLongitude, mLatitude;
            public:
                Sphere();
                void setGeometricParameters(int, int, float = 0.5);
                void initialize();
        };

        class Cylinder: public RigidBody {

        };

        class Rectangle: public RigidBody {
            private:
                int mRows, mCols;
                float mLength, mWidth;
            public:
                Rectangle();
                void setGeometricParameters(int, int, float = 0.5, float = 0.5);
                void initialize();
        };

    }
}

#endif // RIGID_BODY_HPP