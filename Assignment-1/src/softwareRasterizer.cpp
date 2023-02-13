#include "sw.hpp"
#include <iostream>
#include <thread>

namespace COL781 {
    namespace Software {
        // Static constants
        const int SoftwareRasterizer::FRAME_WIDTH;
        const int SoftwareRasterizer::FRAME_HEIGHT;
        const int SoftwareRasterizer::DISPLAY_SCALE;
        const int SoftwareRasterizer::SAMPLE_SIDE;
        
        // Implement helper methods (private)

        // Computes cross product of two 2D vectors (scalar)
        float SoftwareRasterizer::crossProduct2D(glm::vec4 a, glm::vec4 b) {
            return a.x * b.y - a.y * b.x;
        }

        // Computes bounding box of given set of vertices (at pixel level)
        void SoftwareRasterizer::boundingBox2D(glm::vec4 *pVertices, int numVertices, int *minX, int *maxX, int *minY, int *maxY) {
            for (int i = 0; i < numVertices; i ++) {
                *minX = std::min(*minX, (int) pVertices[i].x);
                *maxX = std::max(*maxX, (int) (pVertices[i].x + 1));
                *minY = std::min(*minY, (int) pVertices[i].y);
                *maxY = std::max(*maxY, (int) (pVertices[i].y + 1));
            }
        }

        // Orients points of a triangle in counter-clockwise order
        void SoftwareRasterizer::orientCounterClockwise(Vertex *pVertices) {
            glm::vec4 ab = pVertices[1].position - pVertices[0].position;
            glm::vec4 ac = pVertices[2].position - pVertices[0].position;
            if (crossProduct2D(ab, ac) < 0) {
                Vertex temp = pVertices[1];
                pVertices[1] = pVertices[2];
                pVertices[2] = temp;
            }
        }

        // Checks if a point is inside given triangle (left-handed screen system)
        bool SoftwareRasterizer::isInTriangle(glm::vec4 *pVertices, glm::vec4 point) {
            glm::vec4 ap = point - pVertices[0], ab = pVertices[1] - pVertices[0];
            glm::vec4 bp = point - pVertices[1], bc = pVertices[2] - pVertices[1];
            glm::vec4 cp = point - pVertices[2], ca = pVertices[0] - pVertices[2];
            if (crossProduct2D(ab, ap) > 0 || crossProduct2D(bc, bp) > 0 || crossProduct2D(ca, cp) > 0) {
                return false;
            }
            return true;
        }

        // Computes distance between a point and a line (2D)
        float SoftwareRasterizer::distance(glm::vec2 q, glm::vec2 p[2]) {
            return abs(q.x * (p[0].y - p[1].y) + q.y * (p[1].x - p[0].x) + p[0].x * p[1].y - p[0].y * p[1].x);
        }


        // Returns barycentric coordinate of the point w.r.t given vertex
        float SoftwareRasterizer::phi(int index, glm::vec2 point, glm::vec4 triangle[3]) {
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

        // Implement API (public)

        // Initialize non-SDL members
        SoftwareRasterizer::SoftwareRasterizer(int *pFrameWidth, int *pFrameHeight, int *pDisplayScale, int *pSampleCount) {
            mFrameWidth = pFrameWidth ? *pFrameWidth : FRAME_WIDTH;
            mFrameHeight = pFrameHeight ? *pFrameHeight : FRAME_HEIGHT;
            mDisplayScale = pDisplayScale ? *pDisplayScale : DISPLAY_SCALE;
            mSampleSide = pSampleCount ? ((int) sqrt(*pSampleCount)) : SAMPLE_SIDE;
            mScreenWidth = mFrameWidth * mDisplayScale;
            mScreenHeight = mFrameHeight * mDisplayScale;
            mDepthTesting = false;
            mPShaderProgram = nullptr;
            mVertexCount = 0;
            mVertices.clear();
            mTriangleCount = 0;
            mFragments.clear();
            glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
            glm::mat4 scaleAndInvert = glm::scale(glm::mat4(1.0f), glm::vec3(mFrameWidth / 2.0f, - mFrameHeight / 2.0f, 1.0f));
            glm::mat4 translateBackToOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, mFrameHeight, 0.0f));
            mNDC2dToFrame = translateBackToOrigin * scaleAndInvert * translateToOrigin;
            mSDLActive = false;
            mCBuffer.clear();
            mCBuffer.resize(mFrameHeight);
            for (int i = 0; i < mFrameHeight; i ++) {
                mCBuffer[i].resize(mFrameWidth);
                for (int j = 0; j < mFrameWidth; j ++) {
                    mCBuffer[i][j].resize(mSampleSide, std::vector<glm::vec4>(mSampleSide, glm::vec4(1.0f)));
                }
            }
            mZBuffer.clear();
            mZBuffer.resize(mFrameHeight);
            for (int i = 0; i < mFrameHeight; i ++) {
                mZBuffer[i].resize(mFrameWidth);
                for (int j = 0; j < mFrameWidth; j ++) {
                    mZBuffer[i][j].resize(mSampleSide, std::vector<float>(mSampleSide, 1.0f));
                }
            }
        }

        // Terminate SDL before destruction (if not done already)
        SoftwareRasterizer::~SoftwareRasterizer() {
            terminateSDL();
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

        // Clear buffers (C and Z)
        void SoftwareRasterizer::clearBuffer(glm::vec4 normalizedColor) {
            for (int i = 0; i < mFrameHeight; i ++) {
                for (int j = 0; j < mFrameWidth; j ++) {
                    for (int x = 0; x < mSampleSide; x ++) {
                        for (int y = 0; y < mSampleSide; y ++) {
                            mCBuffer[i][j][x][y] = normalizedColor;
                            if (mDepthTesting) {
                                mZBuffer[i][j][x][y] = 1.0f;
                            }
                        }
                    }
                }
            }
        }

        // Update buffers (C and Z)
        void SoftwareRasterizer::updateBuffer() {
            for (int i = 0; i < mTriangleCount; i ++) {
                for (Fragment &fragment : mFragments[i]) {
                    int p_y = fragment.pixelLocation.y;
                    int p_x = fragment.pixelLocation.x;
                    int n_y = fragment.subPixelLocation.y;
                    int n_x = fragment.subPixelLocation.x;
                    if (mDepthTesting) {
                        if (fragment.depth <= mZBuffer[p_y][p_x][n_y][n_x]) {
                            mCBuffer[p_y][p_x][n_y][n_x] = fragment.color;
                            mZBuffer[p_y][p_x][n_y][n_x] = fragment.depth;
                        }
                    } else {
                        mCBuffer[p_y][p_x][n_y][n_x] = fragment.color;
                    }
                }
            }
        }

        // Transfer buffers (C) to framebuffer
        void SoftwareRasterizer::transferBuffer() {
            if (!mSDLActive) {
                printf("Could not transfer buffer. SDL was not initialized properly!\n");
                return;
            }
            Uint32 *pixels = (Uint32*) mPFramebuffer->pixels;
            SDL_PixelFormat *format = mPFramebuffer->format;
            for (int i = 0; i < mFrameHeight; i ++) {
                for (int j = 0; j < mFrameWidth; j ++) {
                    glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
                    for (std::vector<glm::vec4> &v : mCBuffer[i][j]) {
                        for (glm::vec4 &c : v) {
                            color += c;
                        }
                    }
                    color /= (mSampleSide) * (mSampleSide);
                    color *= 255.0f;
                    pixels[j + mFrameWidth * i] = SDL_MapRGBA(format, (Uint8) color[0], (Uint8) color[1], (Uint8) color[2], (Uint8) color[3]);
                }
            }
        }

        // Show framebuffer on screen/window
        void SoftwareRasterizer::showFramebuffer() {
            if (!mSDLActive) {
                printf("Could not show framebuffer. SDL was not initialized properly!\n");
                return;
            }
            SDL_BlitScaled(mPFramebuffer, NULL, mPWindowSurface, NULL);
            SDL_UpdateWindowSurface(mPWindow);
        }

        // Process vertices of an object
        void SoftwareRasterizer::processVertices() {
            for (int i = 0; i < mVertexCount; i ++) {
                glm::vec4 temp = mPShaderProgram->vs(mPShaderProgram->uniforms, mPObject->attributes[i], *mVertices[i].attributes);
                mVertices[i].position = glm::vec4(temp.x / temp.w, temp.y / temp.w, temp.z / temp.w, 1.0f);
                mVertices[i].wValue = temp.w;
            }
        }

        // Rasterize all triangles
        void SoftwareRasterizer::rasterizeTriangles() {
            std::vector<std::thread> threads(mTriangleCount);
            std::vector<std::vector<Vertex>> vertices(mTriangleCount, std::vector<Vertex>(3));
            for (int i = 0; i < mTriangleCount; i ++) {
                vertices[i][0] = mVertices[mPObject->indices[i][0]];
                vertices[i][1] = mVertices[mPObject->indices[i][1]];
                vertices[i][2] = mVertices[mPObject->indices[i][2]];
                threads[i] = std::thread(&SoftwareRasterizer::rasterizeTriangle, this, i, vertices[i].data());
            }
            for (int i = 0; i < mTriangleCount; i ++) {
                threads[i].join();
            }
        }

        // Rasterize a triangle
        void SoftwareRasterizer::rasterizeTriangle(int index, Vertex vertices[3]) {
            orientCounterClockwise(vertices);
            int attributesCount = vertices[0].attributes->count();
            glm::vec4 screenVertices[3];
            for (int k = 0; k < 3; k ++) {
                screenVertices[k] = mNDC2dToFrame * vertices[k].position;
            }
            int minX = mFrameWidth, maxX = -1;
            int minY = mFrameHeight, maxY = -1;
            boundingBox2D(screenVertices, 3, &minX, &maxX, &minY, &maxY);
            float sampleSpace = 1.0f / (mSampleSide + 1);
            for (int i = std::max(0, minY); i < std::min(maxY, mFrameHeight); i ++) {
                for (int j = std::max(0, minX); j < std::min(maxX, mFrameWidth); j ++) {
                    for (int n_y = 1; n_y <= mSampleSide; n_y ++) {
                        for (int n_x = 1; n_x <= mSampleSide; n_x ++) {
                            float x = j + n_x * sampleSpace;
                            float y = i + n_y * sampleSpace;
                            if (isInTriangle(screenVertices, glm::vec4(x, y, 0.0, 1.0f))) {
                                Fragment fragment;
                                fragment.pixelLocation = glm::ivec2(j, i);
                                fragment.subPixelLocation = glm::ivec2(n_x - 1, n_y - 1);
                                fragment.attributes = new Attribs();
                                float weight[3];
                                float inverseZ = 0.0f;
                                fragment.depth = 0.0f;
                                for (int k = 0; k < 3; k ++) {
                                    weight[k] = phi(k, glm::vec2(x, y), screenVertices);
                                    fragment.depth += weight[k] * vertices[k].position.z;
                                    inverseZ += weight[k] / vertices[k].wValue;
                                }
                                for (int a = 0; a < attributesCount; a ++) {
                                    float temp_f = 0.0f;
                                    glm::vec2 temp_v2 = glm::vec2(0.0f);
                                    glm::vec3 temp_v3 = glm::vec3(0.0f);
                                    glm::vec4 temp_v4 = glm::vec4(0.0f);
                                    int dims = vertices[0].attributes->getDims(a);
                                    for (int k = 0; k < 3; k ++) {
                                        if (dims == 1) {
                                            temp_f += weight[k] * vertices[k].attributes->get<float>(a) / vertices[k].wValue;
                                        } else if (dims == 2) {
                                            temp_v2 += weight[k] * vertices[k].attributes->get<glm::vec2>(a) / vertices[k].wValue;
                                        } else if (dims == 3) {
                                            temp_v3 += weight[k] * vertices[k].attributes->get<glm::vec3>(a) / vertices[k].wValue;
                                        } else {
                                            temp_v4 += weight[k] * vertices[k].attributes->get<glm::vec4>(a) / vertices[k].wValue;
                                        }
                                    }
                                    if (dims == 1) {
                                        fragment.attributes->set<float>(a, temp_f / inverseZ);
                                    } else if (dims == 2) {
                                        fragment.attributes->set<glm::vec2>(a, temp_v2 / inverseZ);
                                    } else if (dims == 3) {
                                        fragment.attributes->set<glm::vec3>(a, temp_v3 / inverseZ);
                                    } else {
                                        fragment.attributes->set<glm::vec4>(a, temp_v4 / inverseZ);
                                    }
                                }
                                fragment.depth = (1 + fragment.depth) / 2;
                                mFragments[index].push_back(fragment);
                            }
                        }
                    }    
                }
            }
        }

        // Process fragments of all triangles
        void SoftwareRasterizer::processFragments() {
            for (int i = 0; i < mTriangleCount; i ++) {
                int fragmentCount = mFragments[i].size();
                for (int j = 0; j < fragmentCount; j ++) {
                    mFragments[i][j].color = mPShaderProgram->fs(mPShaderProgram->uniforms, *mFragments[i][j].attributes);
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

        int SoftwareRasterizer::getSampleCount() {
            return mSampleSide * mSampleSide;
        }

        // Get screen width
        int SoftwareRasterizer::getScreenWidth() {
            return mScreenWidth;
        }

        // Get Screen height
        int SoftwareRasterizer::getScreenHeight() {
            return mScreenHeight;
        }

        // Set framebuffer width
        bool SoftwareRasterizer::setFrameWidth(int frameWidth) {
            if (mSDLActive) {
                printf("Could not change framebuffer width. SDL is already initialised!\n");
                return false;
            }
            mFrameWidth = frameWidth;
            mScreenWidth = mFrameWidth * mDisplayScale;
            glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
            glm::mat4 scaleAndInvert = glm::scale(glm::mat4(1.0f), glm::vec3(mFrameWidth / 2.0f, - mFrameHeight / 2.0f, 1.0f));
            glm::mat4 translateBackToOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, mFrameHeight, 0.0f));
            mNDC2dToFrame = translateBackToOrigin * scaleAndInvert * translateToOrigin;
            mCBuffer.clear();
            mCBuffer.resize(mFrameHeight);
            for (int i = 0; i < mFrameHeight; i ++) {
                mCBuffer[i].resize(mFrameWidth);
                for (int j = 0; j < mFrameWidth; j ++) {
                    mCBuffer[i][j].resize(mSampleSide, std::vector<glm::vec4>(mSampleSide, glm::vec4(1.0f)));
                }
            }
            mZBuffer.clear();
            mZBuffer.resize(mFrameHeight);
            for (int i = 0; i < mFrameHeight; i ++) {
                mZBuffer[i].resize(mFrameWidth);
                for (int j = 0; j < mFrameWidth; j ++) {
                    mZBuffer[i][j].resize(mSampleSide, std::vector<float>(mSampleSide, 1.0f));
                }
            }
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
            glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
            glm::mat4 scaleAndInvert = glm::scale(glm::mat4(1.0f), glm::vec3(mFrameWidth / 2.0f, - mFrameHeight / 2.0f, 1.0f));
            glm::mat4 translateBackToOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, mFrameHeight, 0.0f));
            mNDC2dToFrame = translateBackToOrigin * scaleAndInvert * translateToOrigin;
            mCBuffer.clear();
            mCBuffer.resize(mFrameHeight);
            for (int i = 0; i < mFrameHeight; i ++) {
                mCBuffer[i].resize(mFrameWidth);
                for (int j = 0; j < mFrameWidth; j ++) {
                    mCBuffer[i][j].resize(mSampleSide, std::vector<glm::vec4>(mSampleSide, glm::vec4(1.0f)));
                }
            }
            mZBuffer.clear();
            mZBuffer.resize(mFrameHeight);
            for (int i = 0; i < mFrameHeight; i ++) {
                mZBuffer[i].resize(mFrameWidth);
                for (int j = 0; j < mFrameWidth; j ++) {
                    mZBuffer[i][j].resize(mSampleSide, std::vector<float>(mSampleSide, 1.0f));
                }
            }
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

        // Set number of samples per pixel
        bool SoftwareRasterizer::setSampleCount(int sampleCount) {
            if (mSDLActive) {
                printf("Could not change supersampling sample count. SDL is already initialised!\n");
                return false;
            }
            mSampleSide = (int) sqrt(sampleCount);
            for (int i = 0; i < mFrameHeight; i ++) {
                for (int j = 0; j < mFrameWidth; j ++) {
                    mCBuffer[i][j].resize(mSampleSide, std::vector<glm::vec4>(mSampleSide, glm::vec4(1.0f)));
                    mZBuffer[i][j].resize(mSampleSide, std::vector<float>(mSampleSide, 1.0f));
                }
            }
            return true;
        }

        // Set depth testing flag
        void SoftwareRasterizer::setDepthTesting(bool depthTesting) {
            mDepthTesting = depthTesting;
        }

        // Set custom shader program
        void SoftwareRasterizer::setShaderProgram(const ShaderProgram *shaderProgram) {
            mPShaderProgram = shaderProgram;
        }

        // Set object to be drawn
        void SoftwareRasterizer::setObject(const Object *object) {
            mPObject = object;
            for (Vertex &vertex : mVertices) {
                delete vertex.attributes;
            }
            mVertexCount = (int) mPObject->attributes.size();
            mVertices.resize(mVertexCount);
            for (Vertex &vertex : mVertices) {
                vertex.attributes = new Attribs();
            }
            for (std::vector<Fragment> &v : mFragments) {
                for (Fragment &fragment : v) {
                    delete fragment.attributes;
                }
                v.clear();
            }
            mTriangleCount = (int) mPObject->indices.size();
            mFragments.resize(mTriangleCount);
        }
    }
}