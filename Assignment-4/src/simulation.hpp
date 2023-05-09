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
                long long mStepCounter;
                std::vector<M::Mesh*> mObjects;
                std::vector<M::Mesh*> mCloths;
                std::vector<M::Mesh*> mRigidBodies;
                B::Bone *pMRoot;
                std::vector<int> mTimeSteps;
                std::vector<std::vector<float>> mAnimationControls;
            public:
                Simulation();
                void setStep(float);
                void addObject(M::Mesh*);
                void addCharacter(B::Bone*);
                void setKeyframes(std::vector<int>&, std::vector<std::vector<float>>&);
                float getStep();
                long long getStepCounter();
                int getObjectCount();
                M::Mesh* getObject(int);
                B::Bone* getCharacter();
                void collisionUpdate();
        };

    }
}

#endif // SIMULATION_HPP