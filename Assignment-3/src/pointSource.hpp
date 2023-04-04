#ifndef POINT_SOURCE_HPP
#define POINT_SOURCE_HPP

#include <glm/glm.hpp>

class PointSource {
    private:
        glm::vec3 mCoordinate;
        glm::vec3 mIntensity;
        glm::mat4 mWorldToCamera;
    public:
        PointSource();
        void setCoordinate(glm::vec3);
        void setIntensity(glm::vec3);
        void setWorldToCamera(glm::mat4);
        glm::vec3 getCameraCoordinate();
        glm::vec3 getIrradiance(glm::vec3, glm::vec3);
};

#endif // POINT_SOURCE_HPP