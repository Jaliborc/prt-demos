#include "Classes/PdtState.h"
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

using namespace osgGA;
using namespace osgViewer;

int main() {
	// Scene
	Group* scene = new Group;
	Node *hand = osgDB::readNodeFile("../Captures/generated/300 paper poses/poses 100.0001.obj");
    Environment environment("../Ambients/Museum.tga");
	PdtState pdt("../Captures/generated/300 paper poses", hand, environment);

	scene->addChild(hand);
	scene->addChild(environment.background);

    // Shading
    Program *program = new Program;
    program->addShader(osgDB::readShaderFile(Shader::FRAGMENT, "Shaders/rpdt.frag"));
    program->addShader(osgDB::readShaderFile(Shader::FRAGMENT, "Shaders/simple.frag"));

    StateSet* state = hand->getOrCreateStateSet();
    state->setAttributeAndModes(program, StateAttribute::ON);

	// Interface
	NodeTrackerManipulator* manipulator = new NodeTrackerManipulator;
    manipulator->setNode(hand);

    Viewer viewer;
    viewer.setUpViewInWindow(200, 200, 1024, 1024);
    viewer.setCameraManipulator(manipulator);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setSceneData(scene);
    viewer.realize();
    manipulator->setDistance(hand->getBound().radius() * 4);

    // Run
    fmat joints;
    joints << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0;
    joints = joints.t();
    pdt.rbfUpdate(joints);

    return viewer.run();
}