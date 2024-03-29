#ifndef CLOTH_HPP
#define CLOTH_HPP

#include "mesh.hpp"

namespace M = COL781::Mesh;

namespace COL781 {
    namespace Cloth {

        class Cloth: public M::Mesh {
            private:
                glm::mat4 mInitTransform;
                int mCount;
                int mRows, mCols;
                float mLength, mWidth;
                float mMass, mRadius;
                std::pair<float, float> mStruct, mShear, mBend;
                std::vector<glm::vec3> mVelocity;
                std::vector<bool> mFixed;
                bool mPBD;
                int mNumIter;
            public:
                Cloth();
                void update(float);
                void setInitTransform(glm::mat4);
                void setGeometricParameters(int, int, float = 0.5, float = 0.5);
                void setPhysicalParameters(float, float, std::pair<float, float>, std::pair<float, float>, std::pair<float, float>);
                void setFixedPoints(std::vector<bool>&);
                void enablePBD(int);
                void initialize();
                void checkCollision(Mesh*);
        };

    }
}

#endif // CLOTH_HPP