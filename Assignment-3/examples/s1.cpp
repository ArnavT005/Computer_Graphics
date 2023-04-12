#include "../src/rayTracer.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 1;
    RayTracer r(RenderingMode::NORMALS, &frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    DiffuseSphere s[2];
    s[0].setCenter(glm::vec3(0.0f, 0.0f, -2.0f));
    s[0].setRadius(1.0f);
    s[1].setCenter(glm::vec3(0.0f, -101.0f, -2.0f));
    s[1].setRadius(100.0f);
    r.addObject(&s[0]);
    r.addObject(&s[1]);
    r.calibrateCamera(60.0f, 1.0f, glm::vec3(0, 0, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    r.clearBuffer(glm::vec4(1.0f));
    r.traceRays();
    return EXIT_SUCCESS;
}