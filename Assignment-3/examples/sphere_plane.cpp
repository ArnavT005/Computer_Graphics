#include "../src/rayTracer.hpp"
#include "../src/sphere.hpp"
#include "../src/plane.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 1;
    RayTracer r(&frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    r.calibrateCamera(90.0f, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Sphere s;
    s.setCenter(glm::vec3(0.0f, 0.0f, -2.0f));
    s.setRadius(1.0f);
    Plane p;
    p.setPoint(glm::vec3(0.0f, -1.0f, 0.0f));
    p.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    r.addObject(&s);
    r.addObject(&p);
    while (!r.shouldQuit()) {
        r.clearBuffer(glm::vec4(1.0f));
        r.traceRays();
        r.show();
    }
    return EXIT_SUCCESS;
}