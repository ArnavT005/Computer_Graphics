#include "../src/rayTracer.hpp"
#include "../src/sphere.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 1;
    RayTracer r(&frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    Sphere s[2];
    s[0].setCenter(glm::vec3(0.0f, 0.0f, -2.0f));
    s[0].setRadius(1.0f);
    s[1].setCenter(glm::vec3(0.0f, -101.0f, -2.0f));
    s[1].setRadius(100.0f);
    r.addObject(&s[0]);
    r.addObject(&s[1]);
    r.calibrateCamera(90.0f, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    while (!r.shouldQuit()) {
        r.clearBuffer(glm::vec4(1.0f));
        r.traceRays();
        r.show();
    }
    return EXIT_SUCCESS;
}