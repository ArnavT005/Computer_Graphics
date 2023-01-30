#ifndef SOFTWARE_RASTERIZER_HPP
#define SOFTWARE_RASTERIZER_HPP

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace COL781 {
    namespace Software {

        struct ShaderProgram;
        
        class SoftwareRasterizer {
            private:
                // Static constants
                static const int FRAME_WIDTH = 10;
                static const int FRAME_HEIGHT = 10;
                static const int DISPLAY_SCALE = 1;

                // Framebuffer dimensions
                int mFrameWidth;
                int mFrameHeight;
                // Framebuffer to Screen scale factor
                int mDisplayScale;
                // Screen dimensions
                int mScreenWidth;   // mFrameWidth * mDisplayScale
                int mScreenHeight;  // mFrameHeight * mDisplayScale
                // Sample count per pixel
                int mSampleCount;

                // SDL flag
                bool mSDLActive;
                // Anti-aliasing flag
                bool mAntiAliasingActive;

                // Framebuffer surface
                SDL_Surface *mPFramebuffer;
                // Window (Screen) and its surface
                SDL_Window *mPWindow;
                SDL_Surface *mPWindowSurface;

                // Matrices
                glm::mat4 mNormalized2dToScreen; // NDC (2D) to screen (framebuffer) transformation
                glm::mat4 mCustom2d;             // Custom transformation applied to NDC (2D) (set by user)
                /* user should ensure that there is no clipping after transformation */

                // shader program
                const ShaderProgram *mPShader;

                // Helper methods
                float crossProduct2D(glm::vec4, glm::vec4);   // computes cross product of two 2D vectors (scalar)
                void boundingBox2D(glm::vec4*, int, int*, int*, int*, int*); // computes bounding box of given set of vertices
                void orientCounterClockwise(glm::vec4*);    // orients points of a triangle in counter-clockwise order
                bool isInTriangle(glm::vec4*, glm::vec4);   // checks if a point is inside given triangle
                float distance(glm::vec2, glm::vec2*);      // computes distance between a point and a line (2D)
                float phi(int, glm::vec2, glm::vec4*);      // returns barycentric coordinate of the point w.r.t given vertex

            public:
                // Initialize non-SDL members
                SoftwareRasterizer(int* = nullptr, int* = nullptr, int* = nullptr);
                // Initialize SDL parameters
                bool initializeSDL(std::string = "Assignment 1");
                // Free SDL parameters and close SDL
                void terminateSDL();
                // Draw framebuffer to screen/window
                void drawFramebuffer();
                // // Save framebuffer as image
                // void saveFramebuffer(std::string);
                // Clear framebuffer
                void clearFramebuffer(glm::vec4);
                // Rasterize triangle onto the framebuffer
                void rasterizeTriangle2D(glm::vec4[], glm::vec4);
                // Rasterize arbitrary triangulated shape onto the framebuffer
                void rasterizeArbitraryShape2D(glm::vec4[], glm::ivec3[], glm::vec4[], int);
                void rasterizeObject2D(const glm::vec4[], const glm::ivec3[], const glm::vec4[], int);
                // Turn on anti-aliasing
                void turnOnAntiAliasing(int);
                // Turn off anti-aliasing
                void turnOffAntiAliasing();
                // Getters
                int getFrameWidth();
                int getFrameHeight();
                int getDisplayScale();
                glm::mat4 getCustom2d();
                const ShaderProgram* getShader();
                // Setters
                bool setFrameWidth(int);
                bool setFrameHeight(int);
                bool setDisplayScale(int);
                void setCustom2d(glm::mat4);
                void setShader(const ShaderProgram&);
                void deleteShader();
        };    
    }
}

#endif // SOFTWARE_RASTERIZER_HPP