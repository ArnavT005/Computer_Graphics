#include "../src/viewer.hpp"
#include "../src/cloth.hpp"
#include "../src/simulation.hpp"
#include "../src/rigidBody.hpp"
#include "../src/bone.hpp"

#include <iostream>

namespace V = COL781::Viewer;
namespace C = COL781::Cloth;
namespace S = COL781::Simulation;
namespace R = COL781::RigidBody;
namespace B = COL781::Bone;

using namespace glm;

int main() {
    S::Simulation s;
	V::Viewer v;
	if (!v.initialize("Cloth", 640, 480)) {
		return EXIT_FAILURE;
	}
    
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
    upperArm[0].setInitTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-0.175, 0, 0)), glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0)));
    upperArm[0].setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    upperArm[0].setCollisionParameters(0, 0.01);
    upperArm[0].setGeometricParameters(8, 0.175f, 0.0625f);
    upperArm[0].initialize();
    upperArm[1].setInitTransform(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.175, 0, 0)), glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0)));
    upperArm[1].setPhysicalParameters(glm::vec3(0), 0, glm::vec3(0), glm::vec3(0));
    upperArm[1].setCollisionParameters(0, 0.01);
    upperArm[1].setGeometricParameters(8, 0.175f, 0.0625f);
    upperArm[1].initialize();
    B::Bone upperArmBone0, upperArmBone1;
    upperArmBone0.setParent(&torsoBone, 0.3f * glm::vec3(-glm::cos(glm::pi<float>() / 4.0f), glm::sin(glm::pi<float>() / 4.0f), 0.0f), glm::vec3(0, 0, 1), 0);
    upperArmBone0.setCollider(&upperArm[0]);
    torsoBone.addChild(&upperArmBone0);
    upperArmBone1.setParent(&torsoBone, 0.3f * glm::vec3(glm::cos(glm::pi<float>() / 4.0f), glm::sin(glm::pi<float>() / 4.0f), 0.0f), glm::vec3(0, 0, 1), 0);
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
    lowerArmBone0.setParent(&upperArmBone0, glm::vec3(-0.35, 0, 0), glm::vec3(0, 1, 0), 0);
    lowerArmBone0.setCollider(&lowerArm[0]);
    upperArmBone0.addChild(&lowerArmBone0);
    lowerArmBone1.setParent(&upperArmBone1, glm::vec3(0.35, 0, 0), glm::vec3(0, 1, 0), 0);
    lowerArmBone1.setCollider(&lowerArm[1]);
    upperArmBone1.addChild(&lowerArmBone1);

    torsoBone.update(glm::mat4(1.0f));
    s.setStep(0.0009);
    s.addCharacter(&torsoBone);
    std::vector<long long> timeSteps = {0, 5000, 10000, 15000, 20000};
    std::vector<std::vector<float>> animationControls = {
        {0, 0, 0, 0, 0},
        {0, -glm::pi<float>() / 4.0f, 0, glm::pi<float>() / 4.0f, 0},
        {0, -glm::pi<float>() / 2.0f, 0, glm::pi<float>() / 2.0f, 0},
        {0, -glm::pi<float>() / 4.0f, 0, glm::pi<float>() / 4.0f, 0},
        {0, 0, 0, 0, 0},
    };
    s.setKeyframes(timeSteps, animationControls);
    s.interpolateKeyframes();
    v.view(&s);
}