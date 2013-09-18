#include "Classes/PdtState.h"
#include "Classes/MultipassEffects.h"

#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

using namespace osgViewer;
using namespace osgGA;

int main() {
	// Scene
	Program* program = new Program;
	program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/rpdt.frag")),
	program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/main.frag"));

	Node *hand = osgDB::readNodeFile("../Captures/generated/300 paper poses/poses 100.0001.obj");
	Environment environment("../Ambients/Street.tga");
	PdtState pdt("../Captures/generated/300 paper poses", hand, environment);
	Group *scene = MultipassEffects(hand, environment, program);

	// Interface
    NodeTrackerManipulator *manipulator = new NodeTrackerManipulator;
    manipulator->setTrackNode(hand);

    Viewer viewer;
    viewer.setUpViewInWindow(200, 200, 1024, 1024);
    viewer.setCameraManipulator(manipulator);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setSceneData(scene);
    viewer.realize();
    manipulator->setDistance(80);

    // Run
    fmat joints;
    joints << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0;
    joints = joints.t();
    pdt.rbfUpdate(joints);

    /*Timer timer;
    Timer_t last = 0;
    int pose = 0;

    while (!viewer.done()) {
        Timer_t now = timer.tick();
        if (timer.delta_s(last, now) > 1.0) {
            last = now;
            cout << pose << "\n";
            pdt.update(pose++);
        }

        viewer.frame();
    }*/

    return viewer.run();
}