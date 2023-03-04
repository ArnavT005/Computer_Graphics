#include "../src/viewer.hpp"
#include "../src/mesh.hpp"

namespace V = COL781::Viewer;
namespace M = COL781::Mesh;

using namespace glm;

int main() {
    M::Mesh m;
	V::Viewer v;
	if (!v.initialize("Noisy Cube Mesh", 640, 480)) {
		return EXIT_FAILURE;
	}
    m.load("../meshes/noisycube.obj");
    m.smooth(200, 0.33, true, -0.34);
    m.send(v);
	v.view();
}