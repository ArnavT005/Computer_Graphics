#include "../src/viewer.hpp"
#include "../src/cloth.hpp"
#include "../src/simulation.hpp"
#include "../src/rigidBody.hpp"

namespace V = COL781::Viewer;
namespace C = COL781::Cloth;
namespace S = COL781::Simulation;
namespace R = COL781::RigidBody;

using namespace glm;

int main() {
    S::Simulation s;
    C::Cloth c;
	V::Viewer v;
	if (!v.initialize("Cloth", 640, 480)) {
		return EXIT_FAILURE;
	}
    glm::mat4 transform = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.6, -1)), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));
    c.setInitTransform(transform);
    int m = 8, n = 8;
    c.setGeometricParameters(m, n);
    c.setPhysicalParameters(1, 0.05, {500, 100}, {250, 50}, {50, 10});
    std::vector<bool> fixed((m + 1) * (n + 1), false);
    fixed[0] = fixed[n] = true;
    c.setFixedPoints(fixed);
    c.enablePBD(10);
    c.initialize();
    R::Rectangle rect;
    glm::mat4 rectTransform = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.3, 0)), -glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));
    rect.setInitTransform(rectTransform);
    rect.setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    rect.setCollisionParameters(0, 0.01);
    rect.setGeometricParameters(5, 5, 5, 5);
    rect.initialize();
    R::Sphere sphere;
    glm::mat4 sphereTransform = glm::translate(glm::mat4(1.0f), glm::vec3(2, -0.1, -4));
    sphere.setInitTransform(sphereTransform);
    sphere.setPhysicalParameters(0.5f * glm::vec3(-1, 0, 2), 0, glm::vec3(0), glm::vec3(0));
    sphere.setCollisionParameters(0, 0.01);
    sphere.setGeometricParameters(8, 8, 0.2);
    sphere.initialize();
    R::Cylinder cylinder;
    glm::mat4 cylinderTransform = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-0.90, 0.3, -0.15)), 0.0f, glm::vec3(0, 1, 0));
    cylinder.setInitTransform(cylinderTransform);
    cylinder.setPhysicalParameters(glm::vec3(0, 0, 0), 1, glm::vec3(0, 1, 0), glm::vec3(-0.90, 0.3, -0.5));
    cylinder.setCollisionParameters(0, 0.01);
    cylinder.setGeometricParameters(10, 0.5f, 0.125f);
    cylinder.initialize();
    s.setStep(0.0009);
    s.addObject(&c);
    s.addObject(&rect);
    s.addObject(&sphere);
    s.addObject(&cylinder);
    v.view(&s);
}