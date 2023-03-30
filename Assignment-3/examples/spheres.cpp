#include "../src/rayTracer.hpp"
#include "../src/sphere.hpp"

int main() {
    int frameWidth = 12, frameHeight = 8;
    int displayScale = 40, sampleCount = 1;
    RayTracer r(&frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    r.calibrateCamera(90.0f, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Sphere s[2];
    s[0].setCenter(glm::vec3(0.0f, 0.0f, -10.0f));
    s[0].setRadius(9.0f);
    s[1].setCenter(glm::vec3(0.0f, -101.0f, -2.0f));
    s[1].setRadius(100.0f);
    r.addObject(&s[0]);
    r.addObject(&s[1]);
    while (!r.shouldQuit()) {
        r.clearBuffer(glm::vec4(1.0f));
        r.traceRays();
        r.show();
    }
    return EXIT_SUCCESS;
}