#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include "sphere.hpp"
#include "plane.hpp"
#include "box.hpp"
#include "pointSource.hpp"
#include <string>
#include <vector>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

template <typename T>
using array2D = std::vector<std::vector<T>>;

enum RenderingMode {
    NORMALS,        // no light sources
    POINT_SOURCES,  // ray-tracing supported only for metallic/transparent materials
    AREA_SOURCES,   // path-tracing supported for all three material types
};

class RayTracer {
    private:
        static const int FRAME_WIDTH = 10;
        static const int FRAME_HEIGHT = 10;
        static const int DISPLAY_SCALE = 1;
        static const int SAMPLE_SIDE = 1;
        RenderingMode mMode;
        bool mGammaCorrection;
        bool mIgnoreTransparent;
        int mRecursionDepth;
        glm::vec3 mSkyColor;
        glm::vec3 mAmbientRadiance;
        int mSaveInterval;
        int mFrameWidth;
        int mFrameHeight;
        int mDisplayScale;
        int mSampleSide;
        int mScreenWidth;
        int mScreenHeight;
        float mVerticalFOV;
        float mImagePlane;
        glm::vec3 mCameraCenter;
        glm::vec3 mViewingDirection;
        glm::vec3 mUpVector;
        glm::mat4 mWorldToCamera;
        std::vector<PointSource*> mPointSources;
        std::vector<Object*> mObjects;
        bool mSDLActive;
        array2D<array2D<glm::vec4>> mCBuffer;
        SDL_Surface *mPFramebuffer;
        SDL_Window *mPWindow;
        SDL_Surface *mPWindowSurface;
        bool mQuit;

    public:
        RayTracer(RenderingMode, int* = nullptr, int* = nullptr, int* = nullptr, int* = nullptr);
        ~RayTracer();
        bool initializeSDL(std::string = "Assignment 3");
        void terminateSDL();
        void calibrateCamera(float, float, glm::vec3, glm::vec3, glm::vec3);
        void enableShadowTransparency();
        void disableShadowTransparency();
        void enableGammaCorrection();
        void disableGammaCorrection();
        void applyGammaCorrection(glm::vec4&);
        glm::vec3 incidentRadiance(glm::vec3, glm::vec3, int);
        glm::vec3 incidentRadiance(glm::vec3, glm::vec3, float);      
        glm::vec3 sampleHemisphereCosine(glm::vec3, glm::vec3);
        void addPointSource(PointSource*);
        void addObject(Object*);
        bool shouldQuit();
        void traceRays();
        void show();
        void clearBuffer(glm::vec4);
        void transferBuffer();
        void showFramebuffer();
        int getFrameWidth();
        int getFrameHeight();
        int getDisplayScale();
        int getSampleCount();
        int getScreenWidth();
        int getScreenHeight();
        void setRecursionDepth(int);
        void setSkyColor(glm::vec3);
        void setAmbientRadiance(glm::vec3);
        void setSaveInterval(int);
        bool setFrameWidth(int);
        bool setFrameHeight(int);
        bool setDisplayScale(int);
        bool setSampleCount(int);
};


#endif // RAY_TRACER_HPP

