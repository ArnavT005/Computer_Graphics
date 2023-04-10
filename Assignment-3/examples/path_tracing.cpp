#include "../src/rayTracer.hpp"
#include "../src/sphere.hpp"
#include "../src/plane.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 100;
    RayTracer r(RenderingMode::AREA_SOURCES, &frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    r.setRecursionDepth(50);
    r.setSaveInterval(10);
    r.enableGammaCorrection();
    MetallicSphere s;
    s.setCenter(glm::vec3(0.0f, 0.0f, -2.5f));
    s.setRadius(1.0f);
    s.setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(4, 1, 1)));
    s.setFresnelConstant(glm::vec3(180, 40, 29) / 255.0f);
    TransparentSphere s2;
    s2.setCenter(glm::vec3(0.0f, 0.0f, 0.0f));
    s2.setRadius(1.0f);
    s2.setInternalRefractiveIndex(1.2);
    s2.setExternalRefractiveIndex(1);
    MetallicPlane p;
    p.setPoint(glm::vec3(0.0f, -1.2f, 0.0f));
    p.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    p.setFresnelConstant(glm::vec3(0.3, 0.3f, 0.3f));
    r.addObject(&s);
    r.addObject(&s2);
    r.addObject(&p);
    r.calibrateCamera(60.0f, 1.0f, glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    r.clearBuffer(glm::vec4(1.0f));
    r.traceRays();
    return EXIT_SUCCESS;
}