#include "softwareRasterizer.hpp"

// Initialize non-SDL members
SoftwareRasterizer::SoftwareRasterizer(int *pFrameWidth, int *pFrameHeight, int *pDisplayScale) {
    mFrameWidth = pFrameWidth ? *pFrameWidth : FRAME_WIDTH;
    mFrameHeight = pFrameHeight ? *pFrameHeight : FRAME_HEIGHT;
    mDisplayScale = pDisplayScale ? *pDisplayScale : DISPLAY_SCALE;
    mScreenWidth = mFrameWidth * mDisplayScale;
    mScreenHeight = mFrameHeight * mDisplayScale;
    mSDLActive = false;
}

// Initialize SDL parameters
bool SoftwareRasterizer::initializeSDL(std::string windowTitle) {
    mSDLActive = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        mSDLActive = false;
    } else {
        mPWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN);
        if (mPWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            mSDLActive = false;
        } else {
			mPWindowSurface = SDL_GetWindowSurface(mPWindow);
            mPFramebuffer = SDL_CreateRGBSurface(0, mFrameWidth, mFrameHeight, 32, 0, 0, 0, 0);
        }
    }
    return mSDLActive;
}

// Free SDL parameters and close SDL
void SoftwareRasterizer::terminateSDL() {
    if (mSDLActive) {
        SDL_FreeSurface(mPFramebuffer);
        SDL_DestroyWindow(mPWindow);
        SDL_Quit();
        mSDLActive = false;
    }
}

// Draw framebuffer to screen/window
void SoftwareRasterizer::draw() {
    SDL_BlitScaled(mPFramebuffer, NULL, mPWindowSurface, NULL);
    SDL_UpdateWindowSurface(mPWindow);
}

// Get framebuffer width
int SoftwareRasterizer::getFrameWidth() {
    return mFrameWidth;
}

// Get framebuffer height
int SoftwareRasterizer::getFrameHeight() {
    return mFrameHeight;
}

// Get display scale factor
int SoftwareRasterizer::getDisplayScale() {
    return mDisplayScale;
}

// Set framebuffer width
bool SoftwareRasterizer::setFrameWidth(int frameWidth) {
    if (mSDLActive) {
        printf("Could not change framebuffer width. SDL is already initialised!\n");
        return false;
    }
    mFrameWidth = frameWidth;
    mScreenWidth = mFrameWidth * mDisplayScale;
    return true;
}

// Set framebuffer height
bool SoftwareRasterizer::setFrameHeight(int frameHeight) {
    if (mSDLActive) {
        printf("Could not change framebuffer height. SDL is already initialised!\n");
        return false;
    }
    mFrameHeight = frameHeight;
    mScreenHeight = mFrameHeight * mDisplayScale;
    return true;
}

// Set display scale factor
bool SoftwareRasterizer::setDisplayScale(int displayScale) {
    if (mSDLActive) {
        printf("Could not change display scale factor. SDL is already initialised!\n");
        return false;
    }
    mDisplayScale = displayScale;
    mScreenWidth = mFrameWidth * mDisplayScale;
    mScreenHeight = mFrameHeight * mDisplayScale;
    return true;
}