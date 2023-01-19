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
            float color[4] = {0.0f, 0.0f, 1.0f, 1.0f};
			softwareRasterizer.rasterizeCircle(glm::make_vec4(color));
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


