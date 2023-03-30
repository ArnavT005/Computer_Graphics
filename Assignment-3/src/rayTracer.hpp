#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include "shape.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template <typename T>
using array2D = std::vector<std::vector<T>>;

class RayTracer {
    private:
        static const int FRAME_WIDTH = 10;
        static const int FRAME_HEIGHT = 10;
        static const int DISPLAY_SCALE = 1;
        static const int SAMPLE_SIDE = 1;
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
        std::vector<Shape*> mObjects;
        bool mSDLActive;
        array2D<array2D<glm::vec4>> mCBuffer;
        SDL_Surface *mPFramebuffer;
        SDL_Window *mPWindow;
        SDL_Surface *mPWindowSurface;
        bool mQuit;

    public:
        RayTracer(int* = nullptr, int* = nullptr, int* = nullptr, int* = nullptr);
        ~RayTracer();
        bool initializeSDL(std::string = "Assignment 3");
        void terminateSDL();
        void calibrateCamera(float, float, glm::vec3, glm::vec3, glm::vec3);
        void addObject(Shape*);
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
        bool setFrameWidth(int);
        bool setFrameHeight(int);
        bool setDisplayScale(int);
        bool setSampleCount(int);
};


#endif // RAY_TRACER_HPP

