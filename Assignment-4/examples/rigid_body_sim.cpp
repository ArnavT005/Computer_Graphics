#include "../src/viewer.hpp"
#include "../src/rigidBody.hpp"
#include "../src/simulation.hpp"

namespace V = COL781::Viewer;
namespace R = COL781::RigidBody;
namespace S = COL781::Simulation;

using namespace glm;

int main() {
    S::Simulation s;
    R::Sphere sphere;
    R::Rectangle rect;
	V::Viewer v;
	if (!v.initialize("Cloth", 640, 480)) {
		return EXIT_FAILURE;
	}
    glm::mat4 sphereTransform = glm::translate(glm::mat4(1.0f), glm::vec3(1, -0.3, -1));
    sphere.setInitTransform(sphereTransform);
    sphere.setPhysicalParameters(glm::vec3(-1, 0, 1), 0, glm::vec3(0), glm::vec3(0));
    sphere.setGeometricParameters(8, 8, 0.2);
    sphere.initialize();
    glm::mat4 rectTransform = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, 0)), -glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));
    rect.setInitTransform(rectTransform);
    rect.setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    rect.setGeometricParameters(5, 5, 5, 5);
    rect.initialize();
    s.setStep(0.0003);
    s.addObject(&sphere);
    s.addObject(&rect);
    v.view(&s);
}