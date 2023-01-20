#include "softwareRasterizer.hpp"

// Static constants
const int SoftwareRasterizer::FRAME_WIDTH;
const int SoftwareRasterizer::FRAME_HEIGHT;
const int SoftwareRasterizer::DISPLAY_SCALE;


// Initialize non-SDL members
SoftwareRasterizer::SoftwareRasterizer(int *pFrameWidth, int *pFrameHeight, int *pDisplayScale) {
    mFrameWidth = pFrameWidth ? *pFrameWidth : FRAME_WIDTH;
    mFrameHeight = pFrameHeight ? *pFrameHeight : FRAME_HEIGHT;
    mDisplayScale = pDisplayScale ? *pDisplayScale : DISPLAY_SCALE;
    mScreenWidth = mFrameWidth * mDisplayScale;
    mScreenHeight = mFrameHeight * mDisplayScale;
    float normalized2DToFramebufferArray[] = {mFrameWidth / 2.0f, 0, 0, 0, mFrameHeight / 2.0f, 0, mFrameWidth / 2.0f, mFrameHeight / 2.0f, 1};
    mNormalized2DToFramebufferMatrix = glm::make_mat3(normalized2DToFramebufferArray);
    mSDLActive = false;
}

// Initialize SDL parameters
bool SoftwareRasterizer::initializeSDL(std::string windowTitle) {
    if (mSDLActive) {
        printf("SDL is already initialized!\n");
        return true;
    }
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
    } else {
        printf("Could not free resources. SDL was not intialized properly to begin with!\n");
    }
}

// Draw framebuffer to screen/window
void SoftwareRasterizer::drawFramebuffer() {
    if (mSDLActive) {
        SDL_BlitScaled(mPFramebuffer, NULL, mPWindowSurface, NULL);
        SDL_UpdateWindowSurface(mPWindow);
    } else {
        printf("Could not draw framebuffer. SDL not initialized properly!\n");
    }
}

// Save framebuffer as image
void SoftwareRasterizer::saveFramebuffer(std::string outputFile) {
    if (mSDLActive) {
        IMG_SavePNG(mPFramebuffer, outputFile.c_str());
    } else {
        printf("Could not save framebuffer. SDL not initialized properly!\n");
    }
}

// Clear framebuffer
void SoftwareRasterizer::clearFramebuffer(glm::vec4 normalizedColor) {
    Uint32 *pixels = (Uint32*) mPFramebuffer->pixels;
    SDL_PixelFormat *format = mPFramebuffer->format;
    glm::vec4 color = 255.0f * normalizedColor;
    for (int i = 0; i < mFrameWidth; i ++) {
        for (int j = 0; j < mFrameHeight; j ++) {
            pixels[i + mFrameWidth * j] = SDL_MapRGBA(format, (Uint8) color[0], (Uint8) color[1], (Uint8) color[2], (Uint8) color[3]);
        }
    }
}

// Rasterize triangle onto the framebuffer
void SoftwareRasterizer::rasterizeTriangle2D(glm::vec4 vertices3D[], glm::vec4 normalizedColor) {
    glm::vec3 vertices2D[3];
    for (int i = 0; i < 3; i ++) {
        vertices2D[i][0] = vertices3D[i][0];
        vertices2D[i][1] = vertices3D[i][1];
        vertices2D[i][2] = 1.0f;
        vertices2D[i] = mNormalized2DToFramebufferMatrix * vertices2D[i];
    }
    orientCounterClockwise(vertices2D);
    Uint32 *pixels = (Uint32*) mPFramebuffer->pixels;
    SDL_PixelFormat *format = mPFramebuffer->format;
    glm::vec4 color = 255.0f * normalizedColor;
    for (int i = 0; i < mFrameWidth; i ++) {
        for (int j = 0; j < mFrameHeight; j ++) {
            float x = i + 0.5f;
            float y = j + 0.5f;
            if (isInTriangle(vertices2D, glm::vec3({x, y, 1.0f}))) {
                pixels[i + mFrameWidth * (mFrameHeight - 1 - j)] = SDL_MapRGBA(format, (Uint8) color[0], (Uint8) color[1], (Uint8) color[2], (Uint8) color[3]);
            }
        }
    }
}

// Sample method
void SoftwareRasterizer::rasterizeCircle(glm::vec4 normalizedColor) {
    Uint32 *pixels = (Uint32*) mPFramebuffer->pixels;
    SDL_PixelFormat *format = mPFramebuffer->format;
    glm::vec4 color = 255.0f * normalizedColor;
    for (int i = 0; i < mFrameWidth; i ++) {
        for (int j = 0; j < mFrameHeight; j ++) {
            float x = i + 0.5f;
            float y = j + 0.5f;
            if ((x-5)*(x-5) + (y-5)*(y-5) <= 16) {
                pixels[i + mFrameWidth * j] = SDL_MapRGBA(format, (Uint8) color[0], (Uint8) color[1], (Uint8) color[2], (Uint8) color[3]);
            }
        }
    }
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
    mNormalized2DToFramebufferMatrix[0][0] = mFrameWidth / 2.0f;
    mNormalized2DToFramebufferMatrix[2][0] = mFrameWidth / 2.0f;
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
    mNormalized2DToFramebufferMatrix[1][1] = mFrameHeight / 2.0f;
    mNormalized2DToFramebufferMatrix[2][1] = mFrameHeight / 2.0f;
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
    mNormalized2DToFramebufferMatrix[0][0] = mFrameWidth / 2.0f;
    mNormalized2DToFramebufferMatrix[2][0] = mFrameWidth / 2.0f;
    mNormalized2DToFramebufferMatrix[1][1] = mFrameHeight / 2.0f;
    mNormalized2DToFramebufferMatrix[2][1] = mFrameHeight / 2.0f;
    return true;
}
