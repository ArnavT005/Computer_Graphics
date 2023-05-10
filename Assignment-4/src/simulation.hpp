#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "bone.hpp"

namespace M = COL781::Mesh;
namespace B = COL781::Bone;

namespace COL781 {
    namespace Simulation {
        class Simulation {
            private:
                float mStep;
                int mStepCounter;
                std::vector<M::Mesh*> mObjects;
                std::vector<M::Mesh*> mCloths;
                std::vector<M::Mesh*> mRigidBodies;
                B::Bone *pMRoot;
                std::vector<int> mTimeSteps;
                std::vector<std::vector<float>> mAnimationControls;
                std::vector<std::vector<float>> mAnimationControlDerivates;
                int mActiveIndex;
            public:
                Simulation();
                void setStep(float);
                void addObject(M::Mesh*);
                void addCharacter(B::Bone*);
                void setKeyframes(std::vector<int>&, std::vector<std::vector<float>>&);
                void interpolateKeyframes();
                void updateCharacter();
                void updateStepCounter();
                void updateActiveFrame();
                float getStep();
                int getObjectCount();
                M::Mesh* getObject(int);
                std::vector<M::Mesh*> getCharacterObjects();
                std::vector<glm::mat4> getCharacterTransforms();
                void collisionUpdate();
        };

    }
}

#endif // SIMULATION_HPP