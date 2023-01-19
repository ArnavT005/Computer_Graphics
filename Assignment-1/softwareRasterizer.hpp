#ifndef __SOFTWARE_RASTERIZER_H__
#define __SOFTWARE_RASTERIZER_H__

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SoftwareRasterizer {
    private:
        // Constants
        const int FRAME_WIDTH = 10;
        const int FRAME_HEIGHT = 10;
        const int DISPLAY_SCALE = 40;

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


    public:
        // Initialize non-SDL members
        SoftwareRasterizer(int* = nullptr, int* = nullptr, int* = nullptr);
        // Initialize SDL parameters
        bool initializeSDL(std::string = "Assignment 1");
        // Free SDL parameters and close SDL
        void terminateSDL();
        // Draw framebuffer to screen/window
        void drawFrameBuffer();
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