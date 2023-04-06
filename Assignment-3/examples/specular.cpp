#include "../src/rayTracer.hpp"
#include "../src/sphere.hpp"
#include "../src/plane.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 1;
    RayTracer r(RenderingMode::RAY_TRACING, &frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    r.setRecursionDepth(10);
    MetallicSphere s;
    s.setCenter(glm::vec3(0.0f, 0.0f, -2.0f));
    s.setRadius(1.0f);
    s.setAlbedo(glm::vec3(184, 115, 51) / 500.0f);
    s.setFresnelConstant(glm::vec3(0.3, 0.3, 0.3));
    MetallicSphere s2;
    s2.setCenter(glm::vec3(2.0f, 0.0f, -1.0f));
    s2.setRadius(1.0f);
    s2.setAlbedo(glm::vec3(192) / 800.0f);
    s2.setFresnelConstant(glm::vec3(0.3, 0.3, 0.3));
    MetallicPlane p;
    p.setPoint(glm::vec3(0.0f, -1.0f, 0.0f));
    p.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    p.setAlbedo(glm::vec3(0.0f, 0.0f, 0.0f));
    p.setFresnelConstant(glm::vec3(0.3, 0.3f, 0.3f));
    r.addObject(&s);
    r.addObject(&s2);
    r.addObject(&p);
    PointSource source;
    source.setIntensity(glm::vec3(100));
    source.setCoordinate(glm::vec3(0, 0, 2));
    r.addPointSource(&source);
    r.calibrateCamera(90.0f, 1.0f, glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    while (!r.shouldQuit()) {
        r.clearBuffer(glm::vec4(1.0f));
        r.traceRays();
        r.show();
    }
    return EXIT_SUCCESS;
}