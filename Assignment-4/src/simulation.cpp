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
            mAnimationControlDerivates.clear();
            mActiveIndex = 0;
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
            mAnimationControlDerivates.resize(n, std::vector<float>(m));
            for (int i = 0; i < n; i ++) {
                mTimeSteps[i] = timeSteps[i];
                for (int j = 0; j < m; j ++) {
                    mAnimationControls[i][j] = animationControls[i][j];
                }
            }
        }

        void Simulation::interpolateKeyframes() {
            int n = mAnimationControls.size(), m = mAnimationControls[0].size();
            for (int j = 0; j < m; j ++) {
                mAnimationControlDerivates[0][j] = (mAnimationControls[1][j] - mAnimationControls[0][j]) / ((mTimeSteps[1] - mTimeSteps[0]) * mStep);
                mAnimationControlDerivates[n - 1][j] = (mAnimationControls[n - 1][j] - mAnimationControls[n - 2][j]) / ((mTimeSteps[n - 1] - mTimeSteps[n - 2]) * mStep);
            }
            for (int i = 1; i < n - 1; i ++) {
                for (int j = 0; j < m; j ++) {
                    float rightDerivative = (mAnimationControls[i + 1][j] - mAnimationControls[i][j]) / ((mTimeSteps[i + 1] - mTimeSteps[i]) * mStep);
                    float leftDerivative = (mAnimationControls[i][j] - mAnimationControls[i - 1][j]) / ((mTimeSteps[i] - mTimeSteps[i - 1]) * mStep);
                    int leftTime = mTimeSteps[i] - mTimeSteps[i - 1], rightTime = mTimeSteps[i + 1] - mTimeSteps[i];
                    mAnimationControlDerivates[i][j] = (leftTime * rightDerivative + rightTime * leftDerivative) / (leftTime + rightTime);
                }
            }
        }

        void Simulation::updateCharacter() {
            if (mActiveIndex == mTimeSteps.size() - 1) {
                return;
            }
            float timeElapsed = (mStepCounter - mTimeSteps[mActiveIndex]) / (mTimeSteps[mActiveIndex + 1] - mTimeSteps[mActiveIndex]);
            int n = mAnimationControls.size(), m = mAnimationControls[0].size();
            std::vector<float> animationControls(m);
            float t3 = powf(timeElapsed, 3), t2 = powf(timeElapsed, 2), t1 = timeElapsed;
            for (int j = 0; j < m; j ++) {
                animationControls[j] = (2 * t3 - 3 * t2 + 1) * mAnimationControls[mActiveIndex][j] + (t3 - 2 * t2 + t1) * mAnimationControlDerivates[mActiveIndex][j] + (-2 * t3 + 3 * t2) * mAnimationControls[mActiveIndex + 1][j] + (t3 - t2) * mAnimationControlDerivates[mActiveIndex + 1][j];
            }
            int index = 0;
            pMRoot->setAnimationControls(animationControls, index);
            pMRoot->update(glm::mat4(1.0f));
        }

        void Simulation::updateStepCounter() {
            mStepCounter ++;
        }

        void Simulation::updateActiveFrame() {
            if (mActiveIndex == mTimeSteps.size() - 1) {
                return;
            }
            if (mStepCounter == mTimeSteps[mActiveIndex + 1]) {
                mActiveIndex ++;
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