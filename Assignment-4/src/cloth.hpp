#ifndef CLOTH_HPP
#define CLOTH_HPP

#include "mesh.hpp"

namespace M = COL781::Mesh;

namespace COL781 {
    namespace Cloth {

        class Cloth: public M::Mesh {
            private:
                glm::mat4 mInitTransform;
                int mRows, mCols;
                float mLength, mWidth;
                float mMass, mRadius;
                float mStruct, mShear, mBend, mDamp;
                std::vector<glm::vec3> mVelocity;
                std::vector<bool> mFixed;
                bool mPBD;
            public:
                Cloth();
                void update(float);
                void setInitTransform(glm::mat4);
                void setGeometricParameters(int, int, float = 0.5, float = 0.5);
                void setPhysicalParameters(float, float, float, float, float, float);
                void setFixedPoints(std::vector<bool>&);
                void enablePBD();
                void initialize();
        };

    }
}

#endif // CLOTH_HPP