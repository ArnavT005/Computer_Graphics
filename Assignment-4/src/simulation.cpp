#include "simulation.hpp"
#include "cloth.hpp"

namespace C = COL781::Cloth;

namespace COL781 {
    namespace Simulation {
        Simulation::Simulation() {
            mStep = 0.5f;
            mObjects.clear();
            mCloths.clear();
            mRigidBodies.clear();
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

        void Simulation::collisionUpdate() {
            for (M::Mesh *cloth : mCloths) {
                for (M::Mesh *rigidBody : mRigidBodies) {
                    static_cast<C::Cloth*>(cloth)->checkCollision(rigidBody);
                }
            }
        }

    }
}