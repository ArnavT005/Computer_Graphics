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
            public:
                Simulation();
                void setStep(float);
                void addObject(M::Mesh*);
                float getStep();
                int getObjectCount();
                M::Mesh* getObject(int);
        };

    }
}

#endif // SIMULATION_HPP