#include "../src/viewer.hpp"
#include "../src/cloth.hpp"
#include "../src/simulation.hpp"

namespace V = COL781::Viewer;
namespace C = COL781::Cloth;
namespace S = COL781::Simulation;

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
    int m = 4, n = 4;
    c.setGeometricParameters(m, n);
    c.setPhysicalParameters(1, 0.05, {500, 100}, {250, 50}, {50, 10});
    std::vector<bool> fixed((m + 1) * (n + 1), false);
    fixed[0] = fixed[n] = true;
    c.setFixedPoints(fixed);
    // c.enablePBD(10);
    c.initialize();
    s.setStep(0.0003);
    s.addObject(&c);
    v.view(&s);
}