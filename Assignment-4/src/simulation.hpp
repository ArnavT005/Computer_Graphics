#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "mesh.hpp"

namespace M = COL781::Mesh;

namespace COL781 {
    namespace Simulation {
        class Simulation {
            private:
                float mStep;
                std::vector<M::Mesh*> mObjects;
                std::vector<M::Mesh*> mCloths;
                std::vector<M::Mesh*> mRigidBodies;
            public:
                Simulation();
                void setStep(float);
                void addObject(M::Mesh*);
                float getStep();
                int getObjectCount();
                M::Mesh* getObject(int);
                void collisionUpdate();
        };

    }
}

#endif // SIMULATION_HPP