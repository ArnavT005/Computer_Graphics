#include "../src/rayTracer.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 1;
    RayTracer r(RenderingMode::POINT_SOURCES, &frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    r.enableGammaCorrection();
    DiffuseSphere s;
    s.setCenter(glm::vec3(0.0f, 0.0f, -2.0f));
    s.setRadius(1.0f);
    s.setAlbedo(glm::vec3(0.8, 0, 0));
    DiffusePlane p;
    p.setPoint(glm::vec3(0.0f, -1.0f, 0.0f));
    p.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    p.setAlbedo(glm::vec3(0, 0.8, 0));
    DiffuseBox b[3];
    b[0].setPoints(glm::vec3(-2.5, -1, -3), glm::vec3(-1.5, 2, -1));
    b[0].setAlbedo(glm::vec3(0.8, 0.8, 0));
    b[1].setPoints(glm::vec3(1.5, -1, -2), glm::vec3(3.5, 0, -1));
    b[1].setAlbedo(glm::vec3(0, 0.8, 0.8));
    b[2].setPoints(glm::vec3(-1.5, -1, -5), glm::vec3(4, 2, -3.5));
    b[2].setAlbedo(glm::vec3(0.8, 0, 0.8));
    r.addObject(&s);
    r.addObject(&p);
    r.addObject(&b[0]);
    r.addObject(&b[1]);
    r.addObject(&b[2]);
    PointSource source[2];
    source[0].setCoordinate(glm::vec3(0, 5, -2));
    source[0].setIntensity(glm::vec3(50));
    source[1].setCoordinate(glm::vec3(0, 0, 5));
    source[1].setIntensity(glm::vec3(50));
    r.addPointSource(&source[0]);
    r.addPointSource(&source[1]);
    r.calibrateCamera(60.0f, 1.0f, glm::vec3(0, 0, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    r.clearBuffer(glm::vec4(1.0f));
    r.traceRays();
    return EXIT_SUCCESS;
}