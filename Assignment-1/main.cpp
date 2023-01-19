// Rasterization starter code for COL781: Computer Graphics

/* Instructions for execution:
    1. Install SDL2 and SDL2_image libraries
    2. Compile using: g++ starter_code.cpp -I/path/to/SDL2 -lSDL2 -lSDL2_image
       (on Linux or MacOS, it should be sufficient to copy-paste the following:
       g++ starter_code.cpp `pkg-config --cflags --libs SDL2 SDL2_image`
    3. Run using: ./a.out
    4. The rendered frame will be shown in a window, and saved to a file on exit
*/

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
            // Event handling
            handleEvents(quit);

			Uint32 *pixels = (Uint32*)softwareRasterizer.mPFramebuffer->pixels;
			SDL_PixelFormat *format = softwareRasterizer.mPFramebuffer->format;
            for (int i = 0; i < frameWidth; i++) {
                for (int j = 0; j < frameHeight; j++) {
					float x = i + 0.5;
					float y = j + 0.5;
					float r = 4;
					Uint32 color;
					if ((x-5)*(x-5) + (y-5)*(y-5) <= r*r) { // circle
						color = SDL_MapRGBA(format, 0, 153, 0, 255); // green
					} else {
						color = SDL_MapRGBA(format, 255, 255, 255, 255); // white
					}
					pixels[i + frameWidth*j] = color;
				}
            }
            softwareRasterizer.drawFramebuffer();
        }
    }
	std::string outputFile = "out.png";
    softwareRasterizer.saveFramebuffer(outputFile);
    softwareRasterizer.terminateSDL();
    return 0;
}

// Handle window exit 
void handleEvents(bool &quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }
}


