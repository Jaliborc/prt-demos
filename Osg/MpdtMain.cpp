#include "MpdtState.h"
#include "Environment.h"
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

using namespace osgGA;
using namespace osgViewer;

int main() {
	Group* scene = new Group;

	Node *hand = osgDB::readNodeFile("Media/hand.obj");
	StateSet* uniforms = hand->getOrCreateStateSet();

	PdtState pdt("../Captures/generated/100 paper poses", uniforms);
	Environment* environment = new Environment("../Ambients/Museum.tga", scene);

    Program* program = new Program;
    program->addShader(osgDB::readShaderFile(Shader::FRAGMENT, "Shaders/pgt.frag"));

    Uniform* environmentTransfer = new Uniform(Uniform::FLOAT_VEC3, "environmentTransfer", 9);
    for (int i = 0; i < 9; i++)
   	 environmentTransfer->setElement(i, environment->ambient->at(i));

    uniforms->addUniform(environmentTransfer);
    uniforms->setAttributeAndModes(program, StateAttribute::ON);
	pdt.update(7);

	scene->addChild(hand);
	scene->addChild(environment->background);

	// Interface

	NodeTrackerManipulator* manipulator = new NodeTrackerManipulator;
    manipulator->setNode(hand);

    Viewer viewer;
    viewer.setUpViewOnSingleScreen(1);
    viewer.setCameraManipulator(manipulator);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setSceneData(scene);
    viewer.realize();
    manipulator->setDistance(hand->getBound().radius() * 4);

    return viewer.run();
}