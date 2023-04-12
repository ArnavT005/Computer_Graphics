#include "../src/rayTracer.hpp"

int main() {
    int frameWidth = 640, frameHeight = 480;
    int displayScale = 1, sampleCount = 1;
    RayTracer r(RenderingMode::POINT_SOURCES, &frameWidth, &frameHeight, &displayScale, &sampleCount);
    if (!r.initializeSDL()) {
        return EXIT_FAILURE;
    }
    // r.enableGammaCorrection();
    DiffuseSphere s[3];
    s[0].setCenter(glm::vec3(0.0f, 0.0f, -2.0f));
    s[0].setRadius(1.0f);
    s[0].setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2, 1, 1)));
    s[0].setAlbedo(glm::vec3(0, 0.8, 0.8));
    s[1].setCenter(glm::vec3(-4.0f, 0.0f, -2.0f));
    s[1].setRadius(1.0f);
    s[1].setTransform(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0)), glm::vec3(1, 2, 1)));
    s[1].setAlbedo(glm::vec3(0.8, 0.8, 0));
    s[2].setCenter(glm::vec3(0.0f, 0.0f, 0.0f));
    s[2].setRadius(1.0f);
    s[2].setTransform(glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(4, 0, -2)), glm::pi<float>() / 6.0f, glm::vec3(0, 1, 0)), glm::vec3(2, 1, 1)));
    s[2].setAlbedo(glm::vec3(0.8, 0, 0.8));
    DiffusePlane p;
    p.setPoint(glm::vec3(0.0f, -1.0f, 0.0f));
    p.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    p.setAlbedo(glm::vec3(0, 0.8, 0));
    DiffuseBox b[3];
    b[0].setPoints(glm::vec3(-0.5, -1, -1), glm::vec3(0.5, 1, 1));
    b[0].setAlbedo(glm::vec3(0.8, 0, 0.8));
    b[0].setTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0)), glm::pi<float>() / 4.0f, glm::vec3(0, 1, 0)));
    b[1].setPoints(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, 0.5));
    b[1].setTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(2, -0.5, 1)), -glm::pi<float>() / 4.0f, glm::vec3(0, 1, 0)));
    b[1].setAlbedo(glm::vec3(0, 0.8, 0.8));
    b[2].setPoints(glm::vec3(-1.5, -1, -5), glm::vec3(4, 2, -3.5));
    b[2].setPoints(glm::vec3(-2.75, -1.5, -1.25), glm::vec3(2.75, 1.5, 1.25));
    b[2].setTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.8, 0.5, -5)), -glm::pi<float>() / 8.0f, glm::vec3(0, 1, 0)));
    b[2].setAlbedo(glm::vec3(0.8, 0.8, 0.0));
    r.addObject(&s[0]);
    r.addObject(&s[1]);
    r.addObject(&s[2]);
    r.addObject(&p);
    r.addObject(&b[0]);
    r.addObject(&b[1]);
    r.addObject(&b[2]);
    PointSource source[2];
    source[0].setCoordinate(glm::vec3(0, 10, -2));
    source[0].setIntensity(glm::vec3(200));
    source[1].setCoordinate(glm::vec3(0, 0, 10));
    source[1].setIntensity(glm::vec3(200));
    r.addPointSource(&source[0]);
    r.addPointSource(&source[1]);
    r.calibrateCamera(80.0f, 1.0f, glm::vec3(0, 0, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    r.clearBuffer(glm::vec4(1.0f));
    r.traceRays();
    return EXIT_SUCCESS;
}