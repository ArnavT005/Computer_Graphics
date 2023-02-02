#include "softwareRasterizer.hpp"

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

void rasterizeTriangle(SoftwareRasterizer *pSoftwareRasterizer) {
    glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 vertices[3];
    vertices[0] = glm::vec4(-0.7f, -0.9f, 0.0f, 1.0f);
    vertices[1] = glm::vec4(0.95f, -0.5f, 0.0f, 1.0f);
    vertices[2] = glm::vec4(0.32f, 0.81f, 0.0f, 1.0f);
    pSoftwareRasterizer->rasterizeTriangle2D(vertices, color);
}

void rasterizeTick(SoftwareRasterizer *pSoftwareRasterizer) {
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
}

void rasterizeClock(SoftwareRasterizer *pSoftwareRasterizer, Uint32 displayTime) {
    glm::vec4 unitSquare[] = {
        glm::vec4(-0.5, -0.5, 0.0, 1.0),
        glm::vec4(0.5, -0.5, 0.0, 1.0),
        glm::vec4(0.5, 0.5, 0.0, 1.0),
        glm::vec4(-0.5, 0.5, 0.0, 1.0)
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
        glm::vec4(1.0, 1.0, 0.0, 1.0),
        glm::vec4(1.0, 1.0, 0.0, 1.0)
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
        glm::vec4(1.0, 0.0, 1.0, 1.0),
        glm::vec4(1.0, 0.0, 1.0, 1.0)
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



void rasterizeClockGraduations(SoftwareRasterizer *pSoftwareRasterizer) {
    glm::vec4 forks[] = {
        glm::vec4(-0.01, -0.3, 0.0, 1.0),
        glm::vec4(0.01, -0.3, 0.0, 1.0),
        glm::vec4(0.01, 0.3, 0.0, 1.0),
        glm::vec4(-0.01, 0.3, 0.0, 1.0)
    };
    glm::ivec3 indices[] = {
        glm::ivec3(0, 1, 3),
        glm::ivec3(1, 2, 3)
    };
    glm::vec4 colors[] = {
        glm::vec4(1, 0.0, 0.0, 1.0),
        glm::vec4(1, 0.0, 0.0, 1.0)
    };
    glm::vec4 markingColor[] = {
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        glm::vec4(1.0, 1.0, 1.0, 1.0)
    };
    float markingRadius = 0.90f;
    glm::mat4 iScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.25f, 1.0f));
    glm::mat4 iScaleLarge = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.5f, 1.0f));
    for (int i = 0; i < 60; i ++) {
        float markingAngle = glm::radians(i * 6.0f);
        glm::mat4 markingRotate = glm::rotate(glm::mat4(1.0f), markingAngle, glm::vec3(0.0, 0.0, -1.0));
        glm::vec3 markingCenterCoordinate(markingRadius * glm::sin(markingAngle), markingRadius * glm::cos(markingAngle), 0.0f);
        glm::mat4 markingTranslate = glm::translate(glm::mat4(1.0f), markingCenterCoordinate);
        if(i%5==0){
            pSoftwareRasterizer->setCustom2d(markingTranslate * markingRotate * iScaleLarge);
        }
        else{
            pSoftwareRasterizer->setCustom2d(markingTranslate * markingRotate * iScale);
        }
        pSoftwareRasterizer->rasterizeArbitraryShape2D(forks, indices, markingColor, 2);
    }

}

void rasterizeNumbers(SoftwareRasterizer *pSoftwareRasterizer) {
    
glm::vec4 unitSquare[] = {
        glm::vec4(-0.5, -0.5, 0.0, 1.0),
        glm::vec4(0.5, -0.5, 0.0, 1.0),
        glm::vec4(0.5, 0.5, 0.0, 1.0),
        glm::vec4(-0.5, 0.5, 0.0, 1.0)
    };
    glm::ivec3 indices[] = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(2, 3, 0)
    };
    glm::vec4 markingColor[] = {
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        glm::vec4(1.0, 1.0, 1.0, 1.0)
    };
    float markingRadius = 0.70f;
    glm::mat4 markingScale1 = glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.15, 0.15));
    float markingAngle1 = glm::radians(30.0f);
    glm::vec3 markingCenterCoordinate1(markingRadius * glm::sin(markingAngle1), markingRadius * glm::cos(markingAngle1), 0.0f);
    glm::mat4 markingTranslate1 = glm::translate(glm::mat4(1.0f), markingCenterCoordinate1);
    pSoftwareRasterizer->setCustom2d(markingTranslate1 * markingScale1);
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, markingColor, 2);



    glm::mat4 markingScale2 = glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.15, 0.15));
    float markingAngle2 = glm::radians(60.0f);
    glm::vec3 markingCenterCoordinate2((markingRadius * glm::sin(markingAngle2)*0.9) , markingRadius * glm::cos(markingAngle2), 0.0f);
    glm::mat4 markingTranslate2 = glm::translate(glm::mat4(1.0f), markingCenterCoordinate2);
    pSoftwareRasterizer->setCustom2d(markingTranslate2 * markingScale2);
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, markingColor, 2);


    glm::vec3 markingCenterCoordinate3(markingRadius * glm::sin(markingAngle2) , markingRadius * glm::cos(markingAngle2), 0.0f);
    glm::mat4 markingTranslate3 = glm::translate(glm::mat4(1.0f), markingCenterCoordinate3);
    pSoftwareRasterizer->setCustom2d(markingTranslate3 * markingScale2);
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, markingColor, 2);





}


