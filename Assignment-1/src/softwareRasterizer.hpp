#ifndef SOFTWARE_RASTERIZER_HPP
#define SOFTWARE_RASTERIZER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace COL781 {
    namespace Software {
        
        template <typename T>
        using array2D = std::vector<std::vector<T>>;

        struct Attribs;
        struct ShaderProgram;
        struct Object;
        
        struct Vertex {
            glm::vec4 position;
            Attribs attributes;
            float wValue;
        };

        struct Fragment {
            glm::ivec2 pixelLocation;
            glm::ivec2 subPixelLocation;
            Attribs attributes;
            float depth;
            glm::vec4 color;
        };

        class SoftwareRasterizer {

            private:

                // Static constants
                static const int FRAME_WIDTH = 10;
                static const int FRAME_HEIGHT = 10;
                static const int DISPLAY_SCALE = 1;
                static const int SAMPLE_SIDE = 1;


                // Gettable and Settable

                // Framebuffer dimensions
                int mFrameWidth;
                int mFrameHeight;
                // Framebuffer to Screen scale factor
                int mDisplayScale;
                // Sample count per pixel
                int mSampleSide;


                // Gettable only

                // Screen dimensions
                int mScreenWidth;   // mFrameWidth * mDisplayScale
                int mScreenHeight;  // mFrameHeight * mDisplayScale


                // Settable only

                // Depth testing flag
                bool mDepthTesting;
                // shader program
                const ShaderProgram *mPShaderProgram;
                // Object to be drawn
                const Object *mPObject;


                // Neither Gettable nor Settable

                // Object vertices
                int mVertexCount;
                std::vector<Vertex> mVertices;
                // Triangle fragments
                int mTriangleCount;
                std::vector<std::vector<Fragment>> mFragments;
                // NDC (2D) to Frame transformation
                glm::mat4 mNDC2dToFrame;
                // SDL initialization flag
                bool mSDLActive;
                // Color buffer (C-Buffer)
                array2D<array2D<glm::vec4>> mCBuffer;
                // Depth buffer (Z-Buffer)
                array2D<array2D<float>> mZBuffer;
                // Framebuffer surface
                SDL_Surface *mPFramebuffer;
                // Window (Screen) and its surface
                SDL_Window *mPWindow;
                SDL_Surface *mPWindowSurface;


                // Helper methods
                float crossProduct2D(glm::vec4, glm::vec4);   // computes cross product of two 2D vectors (scalar)
                void boundingBox2D(glm::vec4*, int, int*, int*, int*, int*); // computes bounding box of given set of vertices (at pixel level)
                void orientCounterClockwise(glm::vec4*);    // orients points of a triangle in counter-clockwise order
                bool isInTriangle(glm::vec4*, glm::vec4);   // checks if a point is inside given triangle
                float distance(glm::vec2, glm::vec2[2]);      // computes distance between a point and a line (2D)
                float phi(int, glm::vec2, glm::vec4[3]);      // returns barycentric coordinate of the point w.r.t given vertex


            public:

                // Initialize non-SDL members
                SoftwareRasterizer(int* = nullptr, int* = nullptr, int* = nullptr, int* = nullptr);
                // Terminate SDL before destruction (if not done already)
                ~SoftwareRasterizer();
                // Initialize SDL parameters
                bool initializeSDL(std::string = "Assignment 1");
                // Free SDL parameters and close SDL
                void terminateSDL();
                // Clear buffers (C and Z)
                void clearBuffer(glm::vec4);
                // Update buffers (C and Z)
                void updateBuffer();
                // Transfer buffers (C) to framebuffer
                void transferBuffer();
                // Show framebuffer on screen/window
                void showFramebuffer();
                // Process vertices of an object
                void processVertices();
                // Rasterize all triangles
                void rasterizeTriangles();
                // Rasterize a triangle
                void rasterizeTriangle(int, Vertex[3]);
                // Process fragments of all triangles
                void processFragments();
                // Getters
                int getFrameWidth();
                int getFrameHeight();
                int getDisplayScale();
                int getSampleCount();
                int getScreenWidth();
                int getScreenHeight();
                // Setters
                bool setFrameWidth(int);
                bool setFrameHeight(int);
                bool setDisplayScale(int);
                bool setSampleCount(int);
                void setDepthTesting(bool);
                void setShaderProgram(const ShaderProgram*);
                void setObject(const Object*);
        };    
    }
}

#endif // SOFTWARE_RASTERIZER_HPP