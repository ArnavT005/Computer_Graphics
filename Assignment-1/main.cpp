#include "softwareRasterizer.hpp"

void handleEvents(bool&);

int main(int argc, char* args[]) {
    int frameWidth = 10, frameHeight = 10;
    int displayScale = 40;
    SoftwareRasterizer softwareRasterizer(&frameWidth, &frameHeight, &displayScale);
    if (!softwareRasterizer.initializeSDL()) {
        printf("Failed to initialize!");
    } else {
        bool quit = false;
        while (!quit) {
            handleEvents(quit);
            glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
            glm::vec4 vertices[3];
            vertices[0] = glm::vec4(-0.7f, -0.9f, 0.0f, 1.0f);
            vertices[1] = glm::vec4(0.95f, -0.5f, 0.0f, 1.0f);
            vertices[2] = glm::vec4(0.32f, 0.81f, 0.0f, 1.0f);
			softwareRasterizer.rasterizeTriangle2D(vertices, color);
            softwareRasterizer.drawFramebuffer();
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


