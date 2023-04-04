#include "rayTracer.hpp"

#include <iostream>

const int RayTracer::FRAME_WIDTH;
const int RayTracer::FRAME_HEIGHT;
const int RayTracer::DISPLAY_SCALE;
const int RayTracer::SAMPLE_SIDE;

RayTracer::RayTracer(RenderingMode mode, int *pFrameWidth, int *pFrameHeight, int *pDisplayScale, int *pSampleCount) {
    mMode = mode;
    mGammaCorrection = false;
    mFrameWidth = pFrameWidth ? *pFrameWidth : FRAME_WIDTH;
    mFrameHeight = pFrameHeight ? *pFrameHeight : FRAME_HEIGHT;
    mDisplayScale = pDisplayScale ? *pDisplayScale : DISPLAY_SCALE;
    mSampleSide = pSampleCount ? ((int) sqrt(*pSampleCount)) : SAMPLE_SIDE;
    mScreenWidth = mFrameWidth * mDisplayScale;
    mScreenHeight = mFrameHeight * mDisplayScale;
    mVerticalFOV = 0.0f;
    mCameraCenter = glm::vec3(0.0f);
    mViewingDirection = glm::vec3(0.0f);
    mUpVector = glm::vec3(0.0f);
    mWorldToCamera = glm::mat4(1.0f);
    mPointSources.clear();
    mObjects.clear();
    mSDLActive = false;
    mCBuffer.clear();
    mCBuffer.resize(mFrameHeight);
    for (int i = 0; i < mFrameHeight; i ++) {
        mCBuffer[i].resize(mFrameWidth);
        for (int j = 0; j < mFrameWidth; j ++) {
            mCBuffer[i][j].resize(mSampleSide);
            for (int k = 0; k < mSampleSide; k ++) {
                mCBuffer[i][j][k].resize(mSampleSide, glm::vec4(1.0f));
            }
        }
    }
    mQuit = false;
}

RayTracer::~RayTracer() {
    terminateSDL();
}

bool RayTracer::initializeSDL(std::string windowTitle) {
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

void RayTracer::terminateSDL() {
    if (!mSDLActive) {
        printf("Could not free resources. SDL was not intialized properly to begin with!\n");
        return;
    }
    SDL_FreeSurface(mPFramebuffer);
    SDL_DestroyWindow(mPWindow);
    SDL_Quit();
    mSDLActive = false;
}

void RayTracer::calibrateCamera(float verticalFOV, float imagePlane, glm::vec3 cameraCenter, glm::vec3 viewingDirection, glm::vec3 upVector) {
    mVerticalFOV = verticalFOV;
    mImagePlane = imagePlane;
    mCameraCenter = cameraCenter;
    mViewingDirection = viewingDirection;
    mUpVector = upVector;
    glm::vec3 e[3];
    e[0] = glm::normalize(glm::cross(mViewingDirection, mUpVector));
    e[1] = glm::normalize(glm::cross(e[0], mViewingDirection));
    e[2] = glm::normalize(-mViewingDirection);
    glm::mat4 cameraToWorld;
    cameraToWorld[0] = glm::vec4(e[0], 0.0f);
    cameraToWorld[1] = glm::vec4(e[1], 0.0f);
    cameraToWorld[2] = glm::vec4(e[2], 0.0f);
    cameraToWorld[3] = glm::vec4(mCameraCenter, 1.0f);
    mWorldToCamera = glm::inverse(cameraToWorld);
    for (PointSource *pointSource : mPointSources) {
        pointSource->setWorldToCamera(mWorldToCamera);
    }
    for (Object *obj : mObjects) {
        obj->setWorldToCamera(mWorldToCamera);
    }
}

void RayTracer::enableGammaCorrection() {
    mGammaCorrection = true;
}

void RayTracer::disableGammaCorrection() {
    mGammaCorrection = false;
}

void RayTracer::applyGammaCorrection(glm::vec4 &color) {
    for (int i = 0; i < 3; i ++) {
        if (color[i] <= 0.0031308) {
            color[i] *= 12.92f;
        } else {
            color[i] = 1.055f * powf(color[i], 1.0f / 2.4f) - 0.055f;
        }
    }
}

void RayTracer::addPointSource(PointSource *pointSource) {
    mPointSources.push_back(pointSource);
}

void RayTracer::addObject(Object *object) {
    mObjects.push_back(object);
}

bool RayTracer::shouldQuit() {
    return mQuit;
}

void RayTracer::traceRays() {
    float pixelSize = 2 * mImagePlane * glm::tan(glm::radians(mVerticalFOV / 2)) / mFrameHeight;
    for (int i = 0; i < mFrameHeight; i ++) {
        for (int j = 0; j < mFrameWidth; j ++) {
            glm::vec3 point = glm::vec3(pixelSize * (2 * j - mFrameWidth + 1 / mSampleSide) / 2, pixelSize * (mFrameHeight - 2 * i - 1 / mSampleSide) / 2, -1.0f);
            for (int x = 0; x < mSampleSide; x ++) {
                for (int y = 0; y < mSampleSide; y ++) {
                    if (mMode == RenderingMode::NORMALS || mMode == RenderingMode::POINT_SOURCES) {
                        glm::vec3 origin = glm::vec3(0.0f);
                        glm::vec3 direction = point + glm::vec3(x * pixelSize / mSampleSide, y * pixelSize / mSampleSide, 0.0f);
                        float minTValue = std::numeric_limits<float>::infinity();
                        glm::vec3 intersectionPoint = glm::vec3(0.0f);
                        glm::vec3 intersectionNormal = glm::vec3(0.0f);
                        glm::vec3 albedo = glm::vec3(0.0f);
                        bool hit = false;
                        for (Object *obj : mObjects) {
                            if (obj->intersectRay(origin, direction, mImagePlane, minTValue)) {
                                minTValue = obj->getTValue();
                                intersectionPoint = obj->getIntersectionPoint();
                                intersectionNormal = obj->getIntersectionNormal();
                                ShapeType shape = obj->getShape();
                                switch (shape) {
                                    case ShapeType::SPHERE:
                                        albedo = static_cast<DiffuseSphere*>(obj)->getAlbedo();
                                        break;
                                    case ShapeType::PLANE:
                                        albedo = static_cast<DiffusePlane*>(obj)->getAlbedo();
                                        break;
                                    default:
                                        albedo = static_cast<DiffuseBox*>(obj)->getAlbedo();
                                        break;
                                }
                                hit = true;
                            }
                        }
                        if (hit) {
                            if (mMode == RenderingMode::NORMALS) {
                                mCBuffer[i][j][x][y] = glm::vec4(0.5f * (intersectionNormal + 1.0f), 1.0f);
                            } else {
                                glm::vec3 irradiance(0.0f);
                                for (PointSource *pointSource : mPointSources) {
                                    glm::vec3 shadowOrigin = intersectionPoint;
                                    glm::vec3 shadowDirection = pointSource->getCameraCoordinate() - shadowOrigin;
                                    hit = false;
                                    for (Object *obj : mObjects) {
                                        if (obj->intersectRay(shadowOrigin, shadowDirection, 0.001, 1)) {
                                            hit = true;
                                            break;
                                        }
                                    }
                                    if (hit) {
                                        continue;
                                    }
                                    irradiance += pointSource->getIrradiance(intersectionPoint, intersectionNormal);
                                }
                                mCBuffer[i][j][x][y] = glm::max(glm::min(glm::vec4((albedo * irradiance) / glm::pi<float>(), 1.0f), glm::vec4(1.0f)), 0.0f);
                                if (mGammaCorrection) {
                                    applyGammaCorrection(mCBuffer[i][j][x][y]);
                                }
                            }
                        } else {
                            mCBuffer[i][j][x][y] = glm::vec4(glm::vec3(0.0f), 1.0f);
                        }
                    } else if (mMode == RenderingMode::RAY_TRACING) {
                        // add code for ray tracing metallic/transparent materials
                    } else {
                        // add code for path tracing diffuse/metallic/transparent materials
                    }
                }
            }
        }
    }
}

void RayTracer::show() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            mQuit = true;
        }
    }
    transferBuffer();
    showFramebuffer();
}

void RayTracer::clearBuffer(glm::vec4 normalizedColor) {
    for (int i = 0; i < mFrameHeight; i ++) {
        for (int j = 0; j < mFrameWidth; j ++) {
            for (int x = 0; x < mSampleSide; x ++) {
                for (int y = 0; y < mSampleSide; y ++) {
                    mCBuffer[i][j][x][y] = normalizedColor;
                }
            }
        }
    }
}

void RayTracer::transferBuffer() {
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
void RayTracer::showFramebuffer() {
    if (!mSDLActive) {
        printf("Could not show framebuffer. SDL was not initialized properly!\n");
        return;
    }
    SDL_BlitScaled(mPFramebuffer, NULL, mPWindowSurface, NULL);
    SDL_UpdateWindowSurface(mPWindow);
}

int RayTracer::getFrameWidth() {
    return mFrameWidth;
}

int RayTracer::getFrameHeight() {
    return mFrameHeight;
}

int RayTracer::getDisplayScale() {
    return mDisplayScale;
}

int RayTracer::getSampleCount() {
    return mSampleSide * mSampleSide;
}

int RayTracer::getScreenWidth() {
    return mScreenWidth;
}

int RayTracer::getScreenHeight() {
    return mScreenHeight;
}

bool RayTracer::setFrameWidth(int frameWidth) {
    if (mSDLActive) {
        printf("Could not change framebuffer width. SDL is already initialised!\n");
        return false;
    }
    mFrameWidth = frameWidth;
    mScreenWidth = mFrameWidth * mDisplayScale;
    mCBuffer.clear();
    mCBuffer.resize(mFrameHeight);
    for (int i = 0; i < mFrameHeight; i ++) {
        mCBuffer[i].resize(mFrameWidth);
        for (int j = 0; j < mFrameWidth; j ++) {
            mCBuffer[i][j].resize(mSampleSide);
            for (int k = 0; k < mSampleSide; k ++) {
                mCBuffer[i][j][k].resize(mSampleSide, glm::vec4(1.0f));
            }
        }
    }
    return true;
}

bool RayTracer::setFrameHeight(int frameHeight) {
    if (mSDLActive) {
        printf("Could not change framebuffer height. SDL is already initialised!\n");
        return false;
    }
    mFrameHeight = frameHeight;
    mScreenHeight = mFrameHeight * mDisplayScale;
    mCBuffer.clear();
    mCBuffer.resize(mFrameHeight);
    for (int i = 0; i < mFrameHeight; i ++) {
        mCBuffer[i].resize(mFrameWidth);
        for (int j = 0; j < mFrameWidth; j ++) {
            mCBuffer[i][j].resize(mSampleSide);
            for (int k = 0; k < mSampleSide; k ++) {
                mCBuffer[i][j][k].resize(mSampleSide, glm::vec4(1.0f));
            }
        }
    }
    return true;
}

bool RayTracer::setDisplayScale(int displayScale) {
    if (mSDLActive) {
        printf("Could not change display scale factor. SDL is already initialised!\n");
        return false;
    }
    mDisplayScale = displayScale;
    mScreenWidth = mFrameWidth * mDisplayScale;
    mScreenHeight = mFrameHeight * mDisplayScale;
    return true;
}

bool RayTracer::setSampleCount(int sampleCount) {
    if (mSDLActive) {
        printf("Could not change supersampling sample count. SDL is already initialised!\n");
        return false;
    }
    mSampleSide = (int) sqrt(sampleCount);
    for (int i = 0; i < mFrameHeight; i ++) {
        for (int j = 0; j < mFrameWidth; j ++) {
            mCBuffer[i][j].clear();
            mCBuffer[i][j].resize(mSampleSide);
            for (int k = 0; k < mSampleSide; k ++) {
                mCBuffer[i][j][k].resize(mSampleSide, glm::vec4(1.0, 1.0, 1.0, 1.0));
            }
        }
    }
    return true;
}