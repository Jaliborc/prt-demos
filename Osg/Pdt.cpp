#include <osg/PositionAttitudeTransform>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

#include "Classes/PdtState.h"
#include "Classes/MultipassEffects.h"

using namespace osgViewer;
using namespace osgGA;

float Rotation = 0;
int Background = 0;

class KeyboardHandler : public GUIEventHandler {
public:
    KeyboardHandler() : GUIEventHandler(), direction(0), last(0) {}

    virtual void accept(GUIEventHandlerVisitor& v) {v.visit(*this);};
    virtual bool handle(const GUIEventAdapter& event, GUIActionAdapter& _) {
        GUIEventAdapter::EventType type = event.getEventType();
        int key = event.getKey();

       if (type == GUIEventAdapter::KEYDOWN) {
            if (key == GUIEventAdapter::KEY_Left)
               direction = -0.1;
            else if (key ==  GUIEventAdapter::KEY_Right)
                direction = 0.1;
            else if (key == GUIEventAdapter::KEY_Up)
                Background++;
            else if (key == GUIEventAdapter::KEY_Down)
                Background--;

        } else if (type == GUIEventAdapter::KEYUP)
            if (key == GUIEventAdapter::KEY_Left || key == GUIEventAdapter::KEY_Right)
                direction = 0;


        Timer_t now = timer.tick();
        if (timer.delta_s(last, now) > 1.0/29.0) {
            Rotation += direction;
            last = now;
        }
    }

    Timer timer; Timer_t last;
    float direction;
};

int main() {
	// Scene
	Program* program = new Program;
	program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/pdt.frag")),
	program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/main.frag"));

	Node *hand = osgDB::readNodeFile("../Captures/generated/300 paper poses/poses 100.0001.obj");
    PositionAttitudeTransform *transform = new PositionAttitudeTransform;
    transform->setPosition(Vec3(0, 0, -10));
    transform->addChild(hand);

	EnvironmentSet environments;
    environments.add("../Ambients/Museum.tga");
    environments.add("../Ambients/Street.tga");

	PdtState pdt("../Captures/generated/300 paper poses", hand);
	Group *scene = MultipassEffects(transform, environments, program);

	// Interface
    NodeTrackerManipulator *manipulator = new NodeTrackerManipulator;
    Viewer viewer;
    viewer.setUpViewInWindow(200, 400, 1024, 1024);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.addEventHandler(new KeyboardHandler); 
    viewer.setCameraManipulator(manipulator);
    viewer.setSceneData(scene);
    viewer.realize();
    manipulator->setDistance(80);

    // Run
    fmat joints;
    joints << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0;
    joints = joints.t();
    pdt.updatePose(joints);

    Timer timer;
    Timer_t last = 0;

    while (!viewer.done()) {
        transform->setAttitude(Quat(Rotation, Vec3(1, 0, 0)));
        environments.show(Background);
        pdt.updateScene(environments.ambient);
        viewer.frame();
    }

    return viewer.run();
}