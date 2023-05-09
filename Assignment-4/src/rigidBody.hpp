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
                glm::vec3 mRotationAxis, mRotationPoint;
                float mRestitution, mFriction;
            public:
                RigidBody(M::MeshType);
                void update(float);
                void update(glm::mat4);
                void setInitTransform(glm::mat4);
                void setPhysicalParameters(glm::vec3, float, glm::vec3, glm::vec3);
                void setCollisionParameters(float, float);
                virtual void initialize() = 0;
                virtual void checkCollision(glm::vec3&, glm::vec3&, float, float) = 0;
        };

        class Sphere: public RigidBody {
            private:
                float mRadius;
                int mLongitude, mLatitude;
            public:
                Sphere();
                void setGeometricParameters(int, int, float = 0.5);
                void initialize();
                void checkCollision(glm::vec3&, glm::vec3&, float, float);
        };

        class Rectangle: public RigidBody {
            private:
                int mRows, mCols;
                float mLength, mWidth;
            public:
                Rectangle();
                void setGeometricParameters(int, int, float = 0.5, float = 0.5);
                void initialize();
                void checkCollision(glm::vec3&, glm::vec3&, float, float);
        };
        
        class Cylinder: public RigidBody {
            private:
                int mNumSides;
                float mLength, mRadius;
            public:
                Cylinder();
                void setGeometricParameters(int, float = 0.5, float = 0.25);
                void initialize();
                void checkCollision(glm::vec3&, glm::vec3&, float, float);
        };

    }
}

#endif // RIGID_BODY_HPP