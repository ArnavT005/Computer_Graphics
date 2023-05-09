#ifndef BONE_HPP
#define BONE_HPP

#include "rigidBody.hpp"

namespace M = COL781::Mesh;
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
                glm::mat4 getTransform();
            public:
                Bone();
                void setParent(Bone*, glm::vec3, glm::vec3, float);
                void setCollider(R::RigidBody*);
                void addChild(Bone*);
                void update(glm::mat4);
                void setAnimationControls(std::vector<float>&, int&);
                std::vector<float> getAnimationControls();
                std::vector<M::Mesh*> getRigidBodies();
        };
    }
}

#endif // BONE_HPP