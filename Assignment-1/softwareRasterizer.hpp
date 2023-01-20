#ifndef __SOFTWARE_RASTERIZER_H__
#define __SOFTWARE_RASTERIZER_H__

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class SoftwareRasterizer {
    private:
        // Static constants
        static const int FRAME_WIDTH = 10;
        static const int FRAME_HEIGHT = 10;
        static const int DISPLAY_SCALE = 40;

        // Framebuffer dimensions
        int mFrameWidth;
        int mFrameHeight;
        // Framebuffer to Screen scale factor
        int mDisplayScale;
        // Screen dimensions
        int mScreenWidth;   // mFrameWidth * mDisplayScale
        int mScreenHeight;  // mFrameHeight * mDisplayScale

        // SDL active flag
        bool mSDLActive;

        // Framebuffer surface
        SDL_Surface *mPFramebuffer;
        // Window (Screen) and its surface
        SDL_Window *mPWindow;
        SDL_Surface *mPWindowSurface;

        // Matrices
        glm::mat3 mNormalized2DToFramebufferMatrix;

        // Helper methods
        int crossProduct2D(glm::vec3, glm::vec3);   // computes cross product of two 2D vectors (scalar)
        void orientCounterClockwise(glm::vec3*);    // orients points of a triangle in counter-clockwise order
        bool isInTriangle(glm::vec3*, glm::vec3);   // checks if a point is inside given triangle

    public:
        // Initialize non-SDL members
        SoftwareRasterizer(int* = nullptr, int* = nullptr, int* = nullptr);
        // Initialize SDL parameters
        bool initializeSDL(std::string = "Assignment 1");
        // Free SDL parameters and close SDL
        void terminateSDL();
        // Draw framebuffer to screen/window
        void drawFramebuffer();
        // Save framebuffer as image
        void saveFramebuffer(std::string);
        // Clear framebuffer
        void clearFramebuffer(glm::vec4);
        // Rasterize triangle onto the framebuffer
        void rasterizeTriangle2D(glm::vec4[], glm::vec4);
        // Sample method
        void rasterizeCircle(glm::vec4);
        // Getters
        int getFrameWidth();
        int getFrameHeight();
        int getDisplayScale();
        // Setters
        bool setFrameWidth(int);
        bool setFrameHeight(int);
        bool setDisplayScale(int);
};

#endif // __SOFTWARE_RASTERIZER_H__