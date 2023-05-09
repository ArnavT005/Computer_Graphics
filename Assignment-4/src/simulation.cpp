#include "simulation.hpp"
#include "cloth.hpp"

namespace C = COL781::Cloth;

namespace COL781 {
    namespace Simulation {
        Simulation::Simulation() {
            mStep = 0.5f;
            mStepCounter = 0;
            mObjects.clear();
            mCloths.clear();
            mRigidBodies.clear();
            pMRoot == nullptr;
            mTimeSteps.clear();
            mAnimationControls.clear();
        }

        void Simulation::setStep(float step) {
            mStep = step;
        }

        void Simulation::addObject(M::Mesh *pObject) {
            mObjects.push_back(pObject);
            if (pObject->getType() == M::MeshType::CLOTH) {
                mCloths.push_back(pObject);
            } else {
                mRigidBodies.push_back(pObject);
            }
        }

        void Simulation::addCharacter(B::Bone *pRoot) {
            pMRoot = pRoot;
            if (pMRoot == nullptr) {
                return;
            }
            std::vector<M::Mesh*> rigidBodies = pMRoot->getRigidBodies();
            mRigidBodies.insert(mRigidBodies.end(), rigidBodies.begin(), rigidBodies.end());
        }

        void Simulation::setKeyframes(std::vector<int> &timeSteps, std::vector<std::vector<float>> &animationControls) {
            int n = timeSteps.size(), m = animationControls[0].size();
            mTimeSteps.resize(n);
            mAnimationControls.resize(n, std::vector<float>(m));
            for (int i = 0; i < n; i ++) {
                mTimeSteps[i] = timeSteps[i];
                for (int j = 0; j < m; j ++) {
                    mAnimationControls[i][j] = animationControls[i][j];
                }
            }
        }

        float Simulation::getStep() {
            return mStep;
        }

        long long Simulation::getStepCounter() {
            return mStepCounter;
        }

        int Simulation::getObjectCount() {
            return mObjects.size();
        }

        M::Mesh* Simulation::getObject(int index) {
            if (index < mObjects.size()) {
                return mObjects[index];
            }
            return nullptr;
        }

        B::Bone* Simulation::getCharacter() {
            return pMRoot;
        }

        void Simulation::collisionUpdate() {
            for (M::Mesh *cloth : mCloths) {
                for (M::Mesh *rigidBody : mRigidBodies) {
                    static_cast<C::Cloth*>(cloth)->checkCollision(rigidBody);
                }
            }
        }

    }
}