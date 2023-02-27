#include "../src/viewer.hpp"
#include "../src/mesh.hpp"

namespace V = COL781::Viewer;
namespace M = COL781::Mesh;

using namespace glm;

int main() {
    M::Mesh m;
	V::Viewer v;
	if (!v.initialize("Square Mesh", 640, 480)) {
		return EXIT_FAILURE;
	}
    m.createSquareMesh(5, 5);
    m.send(v);
	v.view();
}
