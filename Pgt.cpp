#include "Classes/Pgt.h"
#include "Classes/MultipassEffects.h"
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

using namespace osgViewer;
using namespace osgGA;

int main() {
    // Scene
    Program* program = new Program;
    program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/pgt.frag"));

    Pgt* hand = new Pgt("../Models/hand/fingerBend.npgt");
    Environment environment("../Ambients/Museum.tga");
    Group *scene = MultipassEffects(hand, environment, program);

    // Interface
    NodeTrackerManipulator* manipulator = new NodeTrackerManipulator;
    manipulator->setNode(hand);

    Viewer viewer;
    viewer.setUpViewOnSingleScreen(1);
    viewer.setCameraManipulator(manipulator);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setSceneData(scene);
    viewer.realize();
    manipulator->setDistance(80);

    // Run
    Timer timer;
    Timer_t last = 0;
    int pose = 0;

    while (!viewer.done()) {
        Timer_t now = timer.tick();
        if (timer.delta_s(last, now) > 1.0/29.0) {
            last = now;
            hand->render(pose++, environment.ambient);

            if (pose == hand->numPoses()) pose = 0;
        }

        viewer.frame();
    }

    return viewer.run();
}