#include "softwareRasterizer.hpp"

void handleEvents(bool&);

void drawTriangle(SoftwareRasterizer*);
void drawTick(SoftwareRasterizer*);
void drawClock(SoftwareRasterizer*);

int main(int argc, char* args[]) {
    int frameWidth = 100, frameHeight = 100;
    int displayScale = 4;
    SoftwareRasterizer softwareRasterizer(&frameWidth, &frameHeight, &displayScale);
    if (!softwareRasterizer.initializeSDL()) {
        printf("Failed to initialize!");
    } else {
        bool quit = false;
        softwareRasterizer.clearFramebuffer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        softwareRasterizer.turnOnAntiAliasing(25);
        while (!quit) {
            handleEvents(quit);
            // Draw red triangle
            // drawTriangle(&softwareRasterizer);
            // Draw green tick mark
            drawTick(&softwareRasterizer);
        }
    }
	std::string outputFile = "out.png";
    softwareRasterizer.saveFramebuffer(outputFile);
    softwareRasterizer.terminateSDL();
    return 0;
}

void handleEvents(bool &quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }
}

void drawTriangle(SoftwareRasterizer *pSoftwareRasterizer) {
    glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 vertices[3];
    vertices[0] = glm::vec4(-0.7f, -0.9f, 0.0f, 1.0f);
    vertices[1] = glm::vec4(0.95f, -0.5f, 0.0f, 1.0f);
    vertices[2] = glm::vec4(0.32f, 0.81f, 0.0f, 1.0f);
    pSoftwareRasterizer->rasterizeTriangle2D(vertices, color);
    pSoftwareRasterizer->drawFramebuffer();
}

void drawTick(SoftwareRasterizer *pSoftwareRasterizer) {
    glm::vec4 vertices[] = {
        glm::vec4(-0.8,  0.0, 0.0, 1.0),
        glm::vec4(-0.4, -0.8, 0.0, 1.0),
        glm::vec4( 0.8,  0.8, 0.0, 1.0),
        glm::vec4(-0.4, -0.4, 0.0, 1.0)
    };
    glm::ivec3 indices[] = {
        glm::ivec3(0, 1, 3),
        glm::ivec3(1, 2, 3)
    };
    glm::vec4 colors[] = {
        glm::vec4(0.0, 0.8, 0.0, 1.0),
        glm::vec4(0.0, 0.8, 0.0, 1.0)
    };
    pSoftwareRasterizer->rasterizeArbitraryShape2D(vertices, indices, colors, 2);
    pSoftwareRasterizer->drawFramebuffer();
}


