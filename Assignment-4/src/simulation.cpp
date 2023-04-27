#include "simulation.hpp"

namespace COL781 {
    namespace Simulation {
        Simulation::Simulation() {
            mStep = 0.5f;
            mObjects.clear();
        }

        void Simulation::setStep(float step) {
            mStep = step;
        }

        void Simulation::addObject(M::Mesh *pObject) {
            mObjects.push_back(pObject);
        }

        float Simulation::getStep() {
            return mStep;
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
    }
}