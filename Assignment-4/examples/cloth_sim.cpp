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
    c.setInitTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.6, 1, 1)));
    c.setGeometricParameters(1, 1);
    c.initialize();
    c.send(&v);
    v.view();
}