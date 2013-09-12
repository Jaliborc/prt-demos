#include "MpdtShader.h"

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
}