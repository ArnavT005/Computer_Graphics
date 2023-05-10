#include "../src/viewer.hpp"
#include "../src/cloth.hpp"
#include "../src/simulation.hpp"
#include "../src/rigidBody.hpp"
#include "../src/bone.hpp"

namespace V = COL781::Viewer;
namespace C = COL781::Cloth;
namespace S = COL781::Simulation;
namespace R = COL781::RigidBody;
namespace B = COL781::Bone;

using namespace glm;

int main() {
    S::Simulation s;
	V::Viewer v;
	if (!v.initialize("Character and Cloth", 640, 480)) {
		return EXIT_FAILURE;
	}
    C::Cloth cloth;
    glm::mat4 transform = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.7, -0.1)), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));
    cloth.setInitTransform(transform);
    int m = 12, n = 8;
    cloth.setGeometricParameters(m, n, 0.25, 0.4);
    cloth.setPhysicalParameters(1, 0.05, {500, 100}, {250, 50}, {50, 10});
    std::vector<bool> fixed((m + 1) * (n + 1), false);
    fixed[0] = fixed[n] = true;
    cloth.setFixedPoints(fixed);
    cloth.enablePBD(10);
    cloth.initialize();

    R::Sphere torso;
    torso.setInitTransform(glm::mat4(1.0f));
    torso.setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    torso.setCollisionParameters(0, 0.01);
    torso.setGeometricParameters(8, 8, 0.3f);
    torso.initialize();
    B::Bone torsoBone;
    torsoBone.setParent(nullptr, glm::vec3(0), glm::vec3(0, 1, 0), 0);
    torsoBone.setCollider(&torso);
    
    R::Cylinder upperArm[2];
    upperArm[0].setInitTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-0.15, 0, 0)), glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0)));
    upperArm[0].setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    upperArm[0].setCollisionParameters(0, 0.01);
    upperArm[0].setGeometricParameters(8, 0.15f, 0.0625f);
    upperArm[0].initialize();
    upperArm[1].setInitTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.15, 0, 0)), glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0)));
    upperArm[1].setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    upperArm[1].setCollisionParameters(0, 0.01);
    upperArm[1].setGeometricParameters(8, 0.15f, 0.0625f);
    upperArm[1].initialize();
    B::Bone upperArmBone0, upperArmBone1;
    upperArmBone0.setParent(&torsoBone, 0.3f * glm::vec3(-glm::cos(glm::pi<float>() / 5.0f), glm::sin(glm::pi<float>() / 5.0f), 0.0f), glm::vec3(0, 0, 1), 0);
    upperArmBone0.setCollider(&upperArm[0]);
    torsoBone.addChild(&upperArmBone0);
    upperArmBone1.setParent(&torsoBone, 0.3f * glm::vec3(glm::cos(glm::pi<float>() / 5.0f), glm::sin(glm::pi<float>() / 5.0f), 0.0f), glm::vec3(0, 0, 1), 0);
    upperArmBone1.setCollider(&upperArm[1]);
    torsoBone.addChild(&upperArmBone1);

    R::Cylinder lowerArm[2];
    lowerArm[0].setInitTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-0.175, 0, 0)), glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0)));
    lowerArm[0].setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    lowerArm[0].setCollisionParameters(0, 0.01);
    lowerArm[0].setGeometricParameters(8, 0.175f, 0.05f);
    lowerArm[0].initialize();
    lowerArm[1].setInitTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.175, 0, 0)), glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0)));
    lowerArm[1].setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    lowerArm[1].setCollisionParameters(0, 0.01);
    lowerArm[1].setGeometricParameters(8, 0.175f, 0.05f);
    lowerArm[1].initialize();
    B::Bone lowerArmBone0, lowerArmBone1;
    lowerArmBone0.setParent(&upperArmBone0, glm::vec3(-0.30, 0, 0), glm::vec3(0, 1, 0), 0);
    lowerArmBone0.setCollider(&lowerArm[0]);
    upperArmBone0.addChild(&lowerArmBone0);
    lowerArmBone1.setParent(&upperArmBone1, glm::vec3(0.30, 0, 0), glm::vec3(0, 1, 0), 0);
    lowerArmBone1.setCollider(&lowerArm[1]);
    upperArmBone1.addChild(&lowerArmBone1);

    R::Cylinder head;
    head.setInitTransform(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.15 / glm::sqrt(2), 0)), glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0)), glm::pi<float>() / 4.0f, glm::vec3(0, 0, 1)));
    head.setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    head.setCollisionParameters(0, 0.01);
    head.setGeometricParameters(4, 0.1f, 0.15f);
    head.initialize();
    B::Bone headBone;
    headBone.setParent(&torsoBone, glm::vec3(0, 0.3f, 0), glm::vec3(1, 0, 0), 0);
    headBone.setCollider(&head);
    torsoBone.addChild(&headBone);
    
    torsoBone.update(glm::mat4(1.0f));
    s.setStep(0.0009);
    s.addObject(&cloth);
    s.addCharacter(&torsoBone);
    std::vector<int> timeSteps = {0, 2500, 5000, 7500, 10000, 13000, 15000};
    std::vector<std::vector<float>> animationControls(7, std::vector<float>(6));
    animationControls[0][0] = 0; animationControls[0][1] = 0; animationControls[0][2] = 0; animationControls[0][3] = 0; animationControls[0][4] = 0; animationControls[0][5] = 0;
    animationControls[1][0] = 0; animationControls[1][1] = -glm::pi<float>() / 4.0f; animationControls[1][2] = 0; animationControls[1][3] = -glm::pi<float>() / 4.0f; animationControls[1][4] = 0; animationControls[1][5] = glm::pi<float>() / 4.0f;
    animationControls[2][0] = 0; animationControls[2][1] = -glm::pi<float>() / 4.0f; animationControls[2][2] = 3.0f * glm::pi<float>() / 4.0f; animationControls[2][3] = -glm::pi<float>() / 4.0f; animationControls[2][4] = -3.0f * glm::pi<float>() / 4.0f; animationControls[2][5] = 0;
    animationControls[3][0] = 0; animationControls[3][1] = 0; animationControls[3][2] = 3.0f * glm::pi<float>() / 4.0f; animationControls[3][3] = 0; animationControls[3][4] = -3.0f * glm::pi<float>() / 4.0f; animationControls[3][5] = glm::pi<float>() / 4.0f;
    animationControls[4][0] = 0; animationControls[4][1] = 0; animationControls[4][2] = glm::pi<float>() / 2.0f; animationControls[4][3] = 0; animationControls[4][4] = -glm::pi<float>() / 2.0f; animationControls[4][5] = 0;
    animationControls[5][0] = 0; animationControls[5][1] = glm::pi<float>() / 4.0f; animationControls[5][2] = 0; animationControls[5][3] = glm::pi<float>() / 4.0f; animationControls[5][4] = 0; animationControls[5][5] = glm::pi<float>() / 4.0f;
    animationControls[6][0] = 2.0f * glm::pi<float>(); animationControls[6][1] = 0; animationControls[6][2] = 0; animationControls[6][3] = 0; animationControls[6][4] = 0; animationControls[6][5] = 0;
    s.setKeyframes(timeSteps, animationControls);
    s.interpolateKeyframes();
    v.view(&s);
}