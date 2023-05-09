#ifndef BONE_HPP
#define BONE_HPP

#include "rigidBody.hpp"

namespace R = COL781::RigidBody;

namespace COL781 {
    namespace Bone {
        class Bone {
            private:
                Bone *pMParent;
                glm::vec3 mPosition, mRotationAxis;
                float mJointAngle;
                R::RigidBody *pMCollider;
                std::vector<Bone*> mChildren;
            public:
                Bone();
                void setParent(Bone*, glm::vec3, glm::vec3, float);
                void setCollider(R::RigidBody*);
                void addChild(Bone*);
                void update(float);
                glm::mat4 getTransform();
        };
    }
}

#endif // BONE_HPP