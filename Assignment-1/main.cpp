#include "softwareRasterizer.hpp"
#include "romans.hpp"

void handleEvents(bool&);

void rasterizeTriangle(SoftwareRasterizer*);
void rasterizeTick(SoftwareRasterizer*);
void rasterizeClock(SoftwareRasterizer*, Uint32);
void rasterizeClockGraduations(SoftwareRasterizer*);
void rasterizeNumbers(SoftwareRasterizer*);

int main(int argc, char* args[]) {
    int frameWidth = 600, frameHeight = 600;
    int displayScale = 1;
    SoftwareRasterizer softwareRasterizer(&frameWidth, &frameHeight, &displayScale);
    if (!softwareRasterizer.initializeSDL()) {
        printf("Failed to initialize!");
    } else {
        bool quit = false;
        softwareRasterizer.turnOnAntiAliasing(25);
        Uint32 displayTime = 0;
        while (!quit) {
            handleEvents(quit);
            softwareRasterizer.clearFramebuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            // Draw red triangle
            // rasterizeTriangle(&softwareRasterizer);
            // Draw green tick mark
            // rasterizeTick(&softwareRasterizer);
            // Draw clock            
            rasterizeClock(&softwareRasterizer, displayTime);
            rasterizeClockGraduations(&softwareRasterizer);
            rasterizeNumbers(&softwareRasterizer);
            softwareRasterizer.drawFramebuffer();
            displayTime ++;
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


void rasterizeClock(SoftwareRasterizer *pSoftwareRasterizer, Uint32 displayTime) {
    glm::vec4 unitSquare[] = {
        glm::vec4(-0.5, -0.5, 0.0, 1.0),
        glm::vec4(0.08, -0.08, 0.0, 1.0),
        glm::vec4(0.5, 0.5, 0.0, 1.0),
        glm::vec4(-0.08, 0.08, 0.0, 1.0)
    };
    glm::ivec3 indices[] = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(2, 3, 0)
    };
    glm::vec4 markingColor[] = {
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        glm::vec4(1.0, 1.0, 1.0, 1.0)
    };
    glm::vec4 secondHandColor[] = {
        glm::vec4(0.0, 1.0, 1.0, 1.0),
        glm::vec4(0.0, 1.0, 1.0, 1.0)
    };
    Uint32 secondPassed = displayTime % 60;
    float secondHandRadius = 0.40f;
    glm::mat4 secondHandScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.025f, 0.8f, 1.0f));
    float secondHandAngle = glm::radians(secondPassed * 6.0f);
    glm::mat4 secondHandRotate = glm::rotate(glm::mat4(1.0f), secondHandAngle, glm::vec3(0.0, 0.0, -1.0f));
    glm::vec3 secondHandCenterCoordinate(secondHandRadius * glm::sin(secondHandAngle), secondHandRadius * glm::cos(secondHandAngle), 0.0f);
    glm::mat4 secondHandTranslate = glm::translate(glm::mat4(1.0f), secondHandCenterCoordinate);
    pSoftwareRasterizer->setCustom2d(secondHandTranslate * secondHandRotate * secondHandScale);
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, secondHandColor, 2);
    glm::vec4 minuteHandColor[] = {
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        glm::vec4(1.0, 1.0, 1.0, 1.0)
    };
    float minutePassed = (displayTime / 60.0f) - 60 * (displayTime / 3600);
    float minuteHandRadius = 0.30f;
    glm::mat4 minuteHandScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.6f, 1.0f));
    float minuteHandAngle = glm::radians(minutePassed * 6.0f);
    glm::mat4 minuteHandRotate = glm::rotate(glm::mat4(1.0f), minuteHandAngle, glm::vec3(0.0, 0.0, -1.0f));
    glm::vec3 minuteHandCenterCoordinate(minuteHandRadius * glm::sin(minuteHandAngle), minuteHandRadius * glm::cos(minuteHandAngle), 0.0f);
    glm::mat4 minuteHandTranslate = glm::translate(glm::mat4(1.0f), minuteHandCenterCoordinate);
    pSoftwareRasterizer->setCustom2d(minuteHandTranslate * minuteHandRotate * minuteHandScale);
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, minuteHandColor, 2);
    glm::vec4 hourHandColor[] = {
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        glm::vec4(1.0, 1.0, 1.0, 1.0)
    };
    float hourPassed = displayTime / 3600.0f - 12 * (displayTime / (12 * 3600));
    float hourHandRadius = 0.15f;
    glm::mat4 hourHandScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.075f, 0.3f, 1.0f));
    float hourHandAngle = glm::radians(hourPassed * 30.0f);
    glm::mat4 hourHandRotate = glm::rotate(glm::mat4(1.0f), hourHandAngle, glm::vec3(0.0, 0.0, -1.0f));
    glm::vec3 hourHandCenterCoordinate(hourHandRadius * glm::sin(hourHandAngle), hourHandRadius * glm::cos(hourHandAngle), 0.0f);
    glm::mat4 hourHandTranslate = glm::translate(glm::mat4(1.0f), hourHandCenterCoordinate);
    pSoftwareRasterizer->setCustom2d(hourHandTranslate * hourHandRotate * hourHandScale);
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, hourHandColor, 2);
}




