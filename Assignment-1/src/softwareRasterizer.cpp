#include "softwareRasterizer.hpp"

namespace COL781 {
    namespace Software {
        // Static constants
        const int SoftwareRasterizer::FRAME_WIDTH;
        const int SoftwareRasterizer::FRAME_HEIGHT;
        const int SoftwareRasterizer::DISPLAY_SCALE;

        // Computes cross product of two 2D vectors (scalar)
        float SoftwareRasterizer::crossProduct2D(glm::vec4 a, glm::vec4 b) {
            return a[0] * b[1] - a[1] * b[0];
        }

        // Computes bounding box of given set of vertices
        void SoftwareRasterizer::boundingBox2D(glm::vec4 *pVertices, int numVertices, int *minX, int *maxX, int *minY, int *maxY) {
            for (int i = 0; i < numVertices; i ++) {
                *minX = std::min(*minX, (int) pVertices[i][0]);
                *maxX = std::max(*maxX, (int) (pVertices[i][0] + 1));
                *minY = std::min(*minY, (int) pVertices[i][1]);
                *maxY = std::max(*maxY, (int) (pVertices[i][1] + 1));
            }
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

        // Computes distance between a point and a line (2D)
        float SoftwareRasterizer::distance(glm::vec2 q, glm::vec2 p[]) {
            return abs(q.x * (p[0].y - p[1].y) + q.y * (p[1].x - p[0].x) + p[0].x * p[1].y - p[0].y * p[1].x);
        }


        // Returns barycentric coordinate of the point w.r.t given vertex
        float SoftwareRasterizer::phi(int index, glm::vec2 point, glm::vec4 triangle[]) {
            glm::vec2 p[2], q;
            for (int i = 0, j = 0; i < 3; i ++) {
                if (i == index) {
                    q = glm::vec2(triangle[i].x, triangle[i].y);
                } else {
                    p[j ++] = glm::vec2(triangle[i].x, triangle[i].y);
                }
            }
            return distance(point, p) / distance(q, p);
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
            mCustom2d = glm::mat4(1.0f);
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
            if (!mSDLActive) {
                printf("Could not free resources. SDL was not intialized properly to begin with!\n");
                return;
            }
            SDL_FreeSurface(mPFramebuffer);
            SDL_DestroyWindow(mPWindow);
            SDL_Quit();
            mSDLActive = false;
        }

        // Draw framebuffer to screen/window
        void SoftwareRasterizer::drawFramebuffer() {
            if (!mSDLActive) {
                printf("Could not draw framebuffer. SDL not initialized properly!\n");
                return;
            }
            SDL_BlitScaled(mPFramebuffer, NULL, mPWindowSurface, NULL);
            SDL_UpdateWindowSurface(mPWindow);
        }

        // // Save framebuffer as image
        // void SoftwareRasterizer::saveFramebuffer(std::string outputFile) {
        //     if (!mSDLActive) {
        //         printf("Could not save framebuffer. SDL not initialized properly!\n");
        //         return;
        //     }
        //     IMG_SavePNG(mPFramebuffer, outputFile.c_str());
        // }

        // Clear framebuffer
        void SoftwareRasterizer::clearFramebuffer(glm::vec4 normalizedColor) {
            if (!mSDLActive) {
                printf("Could not clear framebuffer. SDL not initialized properly!\n");
                return;
            }
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
        void SoftwareRasterizer::rasterizeTriangle2D(glm::vec4 normalizedVertices[], glm::vec4 normalizedColor) {
            if (!mSDLActive) {
                printf("Could not rasterize given triangle. SDL not initialized properly!\n");
                return;
            }
            glm::vec4 screenVertices[3];
            for (int i = 0; i < 3; i ++) {
                screenVertices[i] = mNormalized2dToScreen * (mCustom2d * normalizedVertices[i]);
            }
            int minX = mFrameWidth, maxX = -1;
            int minY = mFrameHeight, maxY = -1;
            boundingBox2D(screenVertices, 3, &minX, &maxX, &minY, &maxY);
            orientCounterClockwise(screenVertices);
            Uint32 *pixels = (Uint32*) mPFramebuffer->pixels;
            SDL_PixelFormat *format = mPFramebuffer->format;
            glm::vec4 color = 255.0f * normalizedColor;
            for (int i = minX; i < maxX; i ++) {
                for (int j = minY; j < maxY; j ++) {
                    float pixelSide = 1.0f / sqrt(mSampleCount);
                    float x = i + pixelSide / 2;
                    float y = j + pixelSide / 2;
                    if (!mAntiAliasingActive) { 
                        if (isInTriangle(screenVertices, glm::vec4(x, y, 0.0f, 1.0f))) {
                            pixels[i + mFrameWidth * (mFrameHeight - 1 - j)] = SDL_MapRGBA(format, (Uint8) color[0], (Uint8) color[1], (Uint8) color[2], (Uint8) color[3]);
                        }
                    } else {
                        Uint8 red, green, blue, alpha;
                        SDL_GetRGBA(pixels[i + mFrameWidth * (mFrameHeight - 1 - j)], format, &red, &green, &blue, &alpha);
                        glm::vec4 backgroundColor(red, green, blue, alpha);
                        glm::vec4 colorSum(0.0f, 0.0f, 0.0f, 0.0f);
                        for(int p = 0; p < sqrt(mSampleCount); p ++, x += pixelSide) {
                            y = j + pixelSide / 2;
                            for (int q = 0; q < sqrt(mSampleCount); q ++, y += pixelSide) {
                                if (isInTriangle(screenVertices, glm::vec4(x, y, 0.0f, 1.0f))) {
                                    colorSum += color;
                                } else {
                                    colorSum += backgroundColor;
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
            if (!mSDLActive) {
                printf("Could not rasterize given shape. SDL not initialized properly!\n");
                return;
            }
            glm::vec4 screenVertices[numTriangles + 2];
            for (int i = 0; i < numTriangles + 2; i ++) {
                screenVertices[i] = mNormalized2dToScreen * (mCustom2d * normalizedVertices[i]);
            }
            int minX = mFrameWidth, maxX = -1;
            int minY = mFrameHeight, maxY = -1;
            boundingBox2D(screenVertices, numTriangles + 2, &minX, &maxX, &minY, &maxY);
            glm::vec4 triangle[numTriangles][3];
            glm::vec4 color[numTriangles];
            for (int i = 0; i < numTriangles; i ++) {
                triangle[i][0] = screenVertices[indices[i][0]];
                triangle[i][1] = screenVertices[indices[i][1]];
                triangle[i][2] = screenVertices[indices[i][2]];
                orientCounterClockwise(triangle[i]);
                color[i] = 255.0f * normalizedColor[i];
            }
            Uint32 *pixels = (Uint32*) mPFramebuffer->pixels;
            SDL_PixelFormat *format = mPFramebuffer->format;
            for (int i = minX; i < maxX; i ++) {
                for (int j = minY; j < maxY; j ++) {
                    float pixelSide = 1.0f / sqrt(mSampleCount);
                    float x = i + pixelSide / 2;
                    float y = j + pixelSide / 2;
                    if (!mAntiAliasingActive) {
                        for (int k = 0; k < numTriangles; k ++) {
                            if (isInTriangle(triangle[k], glm::vec4(x, y, 0.0f, 1.0f))) {
                                pixels[i + mFrameWidth * (mFrameHeight - 1 - j)] = SDL_MapRGBA(format, (Uint8) color[k][0], (Uint8) color[k][1], (Uint8) color[k][2], (Uint8) color[k][3]);
                                break;
                            }
                        }
                    } else {
                        Uint8 red, green, blue, alpha;
                        SDL_GetRGBA(pixels[i + mFrameWidth * (mFrameHeight - 1 - j)], format, &red, &green, &blue, &alpha);
                        glm::vec4 backgroundColor(red, green, blue, alpha);
                        glm::vec4 colorSum(0.0f, 0.0f, 0.0f, 0.0f);
                        for(int p = 0; p < sqrt(mSampleCount); p ++, x += pixelSide) {
                            y = j + pixelSide / 2;
                            for (int q = 0; q < sqrt(mSampleCount); q ++, y += pixelSide) {
                                glm::vec4 tempColor = backgroundColor;
                                for (int k = 0; k < numTriangles; k ++) {
                                    if (isInTriangle(triangle[k], glm::vec4(x, y, 0.0f, 1.0f))) {
                                        tempColor = color[k];
                                        break;
                                    }
                                }
                                colorSum += tempColor;
                            }
                        }
                        glm::vec4 colorAvg = colorSum / (float) mSampleCount;
                        pixels[i + mFrameWidth * (mFrameHeight - 1 - j)] = SDL_MapRGBA(format, (Uint8) colorAvg[0], (Uint8) colorAvg[1], (Uint8) colorAvg[2], (Uint8) colorAvg[3]);
                    }
                }
            }
        }

        // Rasterize arbitrary object onto the framebuffer
        void SoftwareRasterizer::rasterizeObject2D(const glm::vec4 normalizedVertices[], const glm::ivec3 indices[], const glm::vec4 normalizedColor[], int numTriangles) {
            if (!mSDLActive) {
                printf("Could not rasterize given shape. SDL not initialized properly!\n");
                return;
            }
            glm::vec4 screenVertices[numTriangles + 2];
            for (int i = 0; i < numTriangles + 2; i ++) {
                screenVertices[i] = mNormalized2dToScreen * (mCustom2d * normalizedVertices[i]);
            }
            int minX = mFrameWidth, maxX = -1;
            int minY = mFrameHeight, maxY = -1;
            boundingBox2D(screenVertices, numTriangles + 2, &minX, &maxX, &minY, &maxY);
            glm::vec4 triangle[numTriangles][3];
            for (int i = 0; i < numTriangles; i ++) {
                triangle[i][0] = screenVertices[indices[i][0]];
                triangle[i][1] = screenVertices[indices[i][1]];
                triangle[i][2] = screenVertices[indices[i][2]];
                orientCounterClockwise(triangle[i]);
            }
            Uint32 *pixels = (Uint32*) mPFramebuffer->pixels;
            SDL_PixelFormat *format = mPFramebuffer->format;
            for (int i = minX; i < maxX; i ++) {
                for (int j = minY; j < maxY; j ++) {
                    float pixelSide = 1.0f / sqrt(mSampleCount);
                    float x = i + pixelSide / 2;
                    float y = j + pixelSide / 2;
                    if (!mAntiAliasingActive) {
                        for (int k = 0; k < numTriangles; k ++) {
                            if (isInTriangle(triangle[k], glm::vec4(x, y, 0.0f, 1.0f))) {
                                glm::vec4 color(0.0, 0.0, 0.0, 1.0);
                                for (int v = 0; v < 3; v ++) {
                                    color += phi(v, glm::vec2(i, j), triangle[k]) * normalizedColor[indices[k][v]];
                                }
                                color *= 255.0f;
                                pixels[i + mFrameWidth * (mFrameHeight - 1 - j)] = SDL_MapRGBA(format, (Uint8) color[0], (Uint8) color[1], (Uint8) color[2], (Uint8) color[3]);
                                break;
                            }
                        }
                    } else {
                        Uint8 red, green, blue, alpha;
                        SDL_GetRGBA(pixels[i + mFrameWidth * (mFrameHeight - 1 - j)], format, &red, &green, &blue, &alpha);
                        glm::vec4 backgroundColor(red, green, blue, alpha);
                        glm::vec4 colorSum(0.0f, 0.0f, 0.0f, 0.0f);
                        for(int p = 0; p < sqrt(mSampleCount); p ++, x += pixelSide) {
                            y = j + pixelSide / 2;
                            for (int q = 0; q < sqrt(mSampleCount); q ++, y += pixelSide) {
                                glm::vec4 tempColor = backgroundColor;
                                for (int k = 0; k < numTriangles; k ++) {
                                    if (isInTriangle(triangle[k], glm::vec4(x, y, 0.0f, 1.0f))) {
                                        glm::vec4 color(0.0, 0.0, 0.0, 1.0);
                                        for (int v = 0; v < 3; v ++) {
                                            color += phi(v, glm::vec2(i, j), triangle[k]) * normalizedColor[indices[k][v]];
                                        }
                                        tempColor = 255.0f * color;
                                        break;
                                    }
                                }
                                colorSum += tempColor;
                            }
                        }
                        glm::vec4 colorAvg = colorSum / (float) mSampleCount;
                        pixels[i + mFrameWidth * (mFrameHeight - 1 - j)] = SDL_MapRGBA(format, (Uint8) colorAvg[0], (Uint8) colorAvg[1], (Uint8) colorAvg[2], (Uint8) colorAvg[3]);
                    }
                }
            }
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

        // Get custom transformation (2D)
        glm::mat4 SoftwareRasterizer::getCustom2d() {
            return mCustom2d;
        }

        // Get custom shader program
        const ShaderProgram* SoftwareRasterizer::getShader() {
            return mPShader;
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

        // Set custom transformation matrix (2D)
        void SoftwareRasterizer::setCustom2d(glm::mat4 custom2d) {
            mCustom2d = custom2d;
        }

        // Set custom shader program
        void SoftwareRasterizer::setShader(const ShaderProgram &shader) {
            mPShader = &shader;
        }

        // Delete custom shader program
        void SoftwareRasterizer::deleteShader() {
            mPShader = nullptr;
        }
    }
}