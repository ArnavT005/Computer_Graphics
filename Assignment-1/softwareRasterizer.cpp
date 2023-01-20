#include "softwareRasterizer.hpp"

// Static constants
const int SoftwareRasterizer::FRAME_WIDTH;
const int SoftwareRasterizer::FRAME_HEIGHT;
const int SoftwareRasterizer::DISPLAY_SCALE;

// Computes cross product of two 2D vectors (scalar)
int SoftwareRasterizer::crossProduct2D(glm::vec4 a, glm::vec4 b) {
    return a[0] * b[1] - a[1] * b[0];
}


// Orients points of a triangle in counter-clockwise order
void SoftwareRasterizer::orientCounterClockwise(glm::vec4 *pVertices) {
    glm::vec4 ab = pVertices[1] - pVertices[0];
    glm::vec4 ac = pVertices[2] - pVertices[0];
    if (crossProduct2D(ab, ac) < 0) {
        glm::vec4 temp = pVertices[1];
        pVertices[1] = pVertices[2];
        pVertices[2] = temp;
    }
}

// Checks if a point is inside given triangle
bool SoftwareRasterizer::isInTriangle(glm::vec4 *pVertices, glm::vec4 point) {
    glm::vec4 ap = point - pVertices[0], ab = pVertices[1] - pVertices[0];
    glm::vec4 bp = point - pVertices[1], bc = pVertices[2] - pVertices[1];
    glm::vec4 cp = point - pVertices[2], ca = pVertices[0] - pVertices[2];
    if (crossProduct2D(ab, ap) < 0 || crossProduct2D(bc, bp) < 0 || crossProduct2D(ca, cp) < 0) {
        return false;
    }
    return true;
}

// Initialize non-SDL members
SoftwareRasterizer::SoftwareRasterizer(int *pFrameWidth, int *pFrameHeight, int *pDisplayScale) {
    mFrameWidth = pFrameWidth ? *pFrameWidth : FRAME_WIDTH;
    mFrameHeight = pFrameHeight ? *pFrameHeight : FRAME_HEIGHT;
    mDisplayScale = pDisplayScale ? *pDisplayScale : DISPLAY_SCALE;
    mScreenWidth = mFrameWidth * mDisplayScale;
    mScreenHeight = mFrameHeight * mDisplayScale;
    mSampleCount = 1;
    mSDLActive = false;
    mAntiAliasingActive = false;
    mNormalized2dToScreen = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(mFrameWidth / 2.0f, mFrameHeight / 2.0f, 1.0f)), glm::vec3(1.0f, 1.0f, 0.0f));
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
            mBackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
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
    mBackgroundColor = color;
    for (int i = 0; i < mFrameWidth; i ++) {
        for (int j = 0; j < mFrameHeight; j ++) {
            pixels[i + mFrameWidth * j] = SDL_MapRGBA(format, (Uint8) color[0], (Uint8) color[1], (Uint8) color[2], (Uint8) color[3]);
        }
    }
}

// Rasterize triangle onto the framebuffer
void SoftwareRasterizer::rasterizeTriangle2D(glm::vec4 normalizedVertices[], glm::vec4 normalizedColor) {
    glm::vec4 screenVertices[3];
    for (int i = 0; i < 3; i ++) {
        screenVertices[i] = mNormalized2dToScreen * normalizedVertices[i];
    }
    orientCounterClockwise(screenVertices);
    Uint32 *pixels = (Uint32*) mPFramebuffer->pixels;
    SDL_PixelFormat *format = mPFramebuffer->format;
    glm::vec4 color = 255.0f * normalizedColor;
    for (int i = 0; i < mFrameWidth; i ++) {
        for (int j = 0; j < mFrameHeight; j ++) {
            float pixelSide = 1.0f / sqrt(mSampleCount);
            float x = i + pixelSide / 2;
            float y = j + pixelSide / 2;
            if (!mAntiAliasingActive) { 
                if (isInTriangle(screenVertices, glm::vec4(x, y, 0.0f, 1.0f))) {
                    pixels[i + mFrameWidth * (mFrameHeight - 1 - j)] = SDL_MapRGBA(format, (Uint8) color[0], (Uint8) color[1], (Uint8) color[2], (Uint8) color[3]);
                }
            } else {
                glm::vec4 colorSum(0.0f, 0.0f, 0.0f, 0.0f);
                for(int p = 0; p < sqrt(mSampleCount); p ++, x += pixelSide) {
                    y = j + pixelSide / 2;
                    for (int q = 0; q < sqrt(mSampleCount); q ++, y += pixelSide) {
                        if (isInTriangle(screenVertices, glm::vec4(x, y, 0.0f, 1.0f))) {
                            colorSum += color;
                        } else {
                            colorSum += mBackgroundColor;
                        }
                    }
                }
                glm::vec4 colorAvg = colorSum / (float) mSampleCount;
                pixels[i + mFrameWidth * (mFrameHeight - 1 - j)] = SDL_MapRGBA(format, (Uint8) colorAvg[0], (Uint8) colorAvg[1], (Uint8) colorAvg[2], (Uint8) colorAvg[3]);
            }
        }
    }
}

// Rasterize arbitrary triangulated shape onto the framebuffer
void SoftwareRasterizer::rasterizeArbitraryShape2D(glm::vec4 normalizedVertices[], glm::ivec3 indices[], glm::vec4 normalizedColor[], int numTriangles) {
    

}

// Turn on anti-aliasing
void SoftwareRasterizer::turnOnAntiAliasing(int sampleCount) {
    mSampleCount = sampleCount;
    mAntiAliasingActive = true;
}

// Turn off anti-aliasing
void SoftwareRasterizer::turnOffAntiAliasing() {
    mSampleCount = 1;
    mAntiAliasingActive = false;
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
    mNormalized2dToScreen = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(mFrameWidth / 2.0f, mFrameHeight / 2.0f, 1.0f)), glm::vec3(1.0f, 1.0f, 0.0f));
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
    mNormalized2dToScreen = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(mFrameWidth / 2.0f, mFrameHeight / 2.0f, 1.0f)), glm::vec3(1.0f, 1.0f, 0.0f));
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
    mNormalized2dToScreen = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(mFrameWidth / 2.0f, mFrameHeight / 2.0f, 1.0f)), glm::vec3(1.0f, 1.0f, 0.0f));
    return true;
}
