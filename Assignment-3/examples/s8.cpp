#include "../src/rayTracer.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 256;
    RayTracer r(RenderingMode::AREA_SOURCES, &frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    r.setRecursionDepth(10);
    r.setSaveInterval(5);
    r.enableGammaCorrection();
    DiffusePlane p[5];
    p[0].setPoint(glm::vec3(0, -3, 0));
    p[0].setNormal(glm::vec3(0, 1, 0));
    p[0].setAlbedo(glm::vec3(0.9));
    p[1].setPoint(glm::vec3(3.5, 0, 0));
    p[1].setNormal(glm::vec3(-1, 0, 0));
    p[1].setAlbedo(glm::vec3(0.1, 0.8, 0.1));
    p[2].setPoint(glm::vec3(-3.5, 0, 0));
    p[2].setNormal(glm::vec3(1, 0, 0));
    p[2].setAlbedo(glm::vec3(0.8, 0.1, 0.1));
    p[3].setPoint(glm::vec3(0, 0, -3));
    p[3].setNormal(glm::vec3(0, 0, 1));
    p[3].setAlbedo(glm::vec3(0.1, 0.1, 0.8));
    p[4].setPoint(glm::vec3(0, 3, 0));
    p[4].setNormal(glm::vec3(0, -1, 0));
    p[4].setAlbedo(glm::vec3(0.9));
    MetallicBox b;
    b.setPoints(glm::vec3(-0.5, -1, -1), glm::vec3(0.5, 1, 1));
    b.setTransform(glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.0f, -1.5f)), -glm::pi<float>() / 3.0f, glm::vec3(0, 1, 0)), glm::vec3(1, 2, 1)));
    b.setFresnelConstant(glm::vec3(0.8));
    MetallicSphere s;
    s.setCenter(glm::vec3(0));
    s.setRadius(1.0f);
    s.setTransform(glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, -2.25, -0.85)), glm::pi<float>() / 4.0f, glm::vec3(0, 1, 0)), glm::vec3(2, 0.75, 0.8)));
    s.setFresnelConstant(glm::vec3(215, 190, 105) / 255.0f);
    TransparentSphere s2;
    s2.setCenter(glm::vec3(1.6f, -1.5f, 1.0f));
    s2.setRadius(1.5f);
    s2.setInternalRefractiveIndex(1.8);
    s2.setExternalRefractiveIndex(1);
    r.addObject(&p[0]);
    r.addObject(&p[1]);
    r.addObject(&p[2]);
    r.addObject(&p[3]);
    r.addObject(&p[4]);
    r.addObject(&b);
    r.addObject(&s);
    r.addObject(&s2);
    BoxSource source;
    source.setPoints(glm::vec3(-2.5, 2.8, -1), glm::vec3(2.5, 4, 2.5));
    source.setRadiance(glm::vec3(1));
    r.addObject(&source);
    r.calibrateCamera(60.0f, 1.0f, glm::vec3(0.0f, 0.0f, 7.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    r.clearBuffer(glm::vec4(1.0f));
    r.traceRays();
    return EXIT_SUCCESS;
}