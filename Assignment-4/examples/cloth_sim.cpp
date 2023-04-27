#include "../src/viewer.hpp"
#include "../src/cloth.hpp"

namespace V = COL781::Viewer;
namespace C = COL781::Cloth;

using namespace glm;

int main() {
    C::Cloth c;
	V::Viewer v;
	if (!v.initialize("Cloth", 640, 480)) {
		return EXIT_FAILURE;
	}
    glm::mat4 transform = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, -0.5)), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));
    c.setInitTransform(transform);
    int m = 1, n = 1;
    c.setGeometricParameters(m, n);
    c.setPhysicalParameters(1, 0.05, 1, 0.5, 0.1, 0.05);
    std::vector<bool> fixed((m + 1) * (n + 1), false);
    fixed[0] = fixed[n] = true;
    c.setFixedPoints(fixed);
    c.initialize();
    c.send(&v);
    v.view();
}