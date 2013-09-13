#include "MpdtShader.h"
#include <osgViewer/Viewer>
using namespace osgViewer;

int main() {
	Node *hand = osgDB::readNodeFile("Media/hand.obj");
	StateSet* uniforms = hand->getOrCreateStateSet();
	PdtShader pdt("../Captures/generated/100 paper poses", uniforms);

    Program* program = new Program;
    program->addShader(osgDB::readShaderFile(Shader::FRAGMENT, "Shaders/pgt.frag"));
    uniforms->setAttributeAndModes(program, StateAttribute::ON);

	cout << "Sucess loading!\n";
	pdt.update(0);
	cout << "Sucess updating!\n";
	pdt.update(1);
	cout << "Sucess updating!\n";

	// Interface
	Viewer viewer;
    viewer.setUpViewOnSingleScreen(1);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setSceneData(hand);
    viewer.realize();
    return viewer.run();
}