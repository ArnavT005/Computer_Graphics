#include "../src/rayTracer.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 1;
    RayTracer r(RenderingMode::POINT_SOURCES, &frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    r.setRecursionDepth(20);
    r.enableShadowTransparency();
    r.enableGammaCorrection();
    DiffusePlane p[5];
    p[0].setPoint(glm::vec3(0, -3, 0));
    p[0].setNormal(glm::vec3(0, 1, 0));
    p[0].setAlbedo(glm::vec3(1));
    p[1].setPoint(glm::vec3(3.5, 0, 0));
    p[1].setNormal(glm::vec3(-1, 0, 0));
    p[1].setAlbedo(glm::vec3(0, 1, 0));
    p[2].setPoint(glm::vec3(-3.5, 0, 0));
    p[2].setNormal(glm::vec3(1, 0, 0));
    p[2].setAlbedo(glm::vec3(1, 0, 0));
    p[3].setPoint(glm::vec3(0, 0, -3));
    p[3].setNormal(glm::vec3(0, 0, 1));
    p[3].setAlbedo(glm::vec3(0, 0, 1));
    p[4].setPoint(glm::vec3(0, 3, 0));
    p[4].setNormal(glm::vec3(0, -1, 0));
    p[4].setAlbedo(glm::vec3(1));
    MetallicSphere s;
    s.setCenter(glm::vec3(-2.0f, -2.0f, -1.5f));
    s.setRadius(1.0f);
    s.setFresnelConstant(glm::vec3(1));
    TransparentSphere s2;
    s2.setCenter(glm::vec3(1.4f, -1.4f, 1.0f));
    s2.setRadius(1.5f);
    s2.setInternalRefractiveIndex(2.5);
    s2.setExternalRefractiveIndex(1);
    r.addObject(&p[0]);
    r.addObject(&p[1]);
    r.addObject(&p[2]);
    r.addObject(&p[3]);
    r.addObject(&p[4]);
    r.addObject(&s);
    r.addObject(&s2);
    PointSource source;
    source.setCoordinate(glm::vec3(0.0f, 2.8f, -0.5f));
    source.setIntensity(glm::vec3(100));
    r.addPointSource(&source);
    r.calibrateCamera(60.0f, 1.0f, glm::vec3(0.0f, 0.0f, 7.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    r.clearBuffer(glm::vec4(1.0f));
    r.traceRays();
    return EXIT_SUCCESS;
}