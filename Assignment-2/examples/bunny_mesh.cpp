#include "../src/viewer.hpp"
#include "../src/mesh.hpp"

namespace V = COL781::Viewer;
namespace M = COL781::Mesh;

using namespace glm;

int main() {
    M::Mesh m;
	V::Viewer v;
	if (!v.initialize("Bunny Mesh", 640, 480)) {
		return EXIT_FAILURE;
	}
    m.load("../meshes/bunny-1k.obj");
    m.send(v);
	v.view();
}
