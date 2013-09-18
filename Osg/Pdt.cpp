#include "Classes/PdtState.h"
#include <osg/PositionAttitudeTransform>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

using namespace osgGA;
using namespace osgViewer;

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
	Group* scene = new Group;
    Node *hand = osgDB::readNodeFile("../Captures/generated/300 paper poses/poses 100.0001.obj");
    PositionAttitudeTransform *transform = new PositionAttitudeTransform;
    transform->setPosition(Vec3(0, 0, -10));
    transform->addChild(hand);

    Environment environment("../Ambients/Museum.tga");
	PdtState pdt("../Captures/generated/300 paper poses", transform, environment);

	scene->addChild(transform);
	scene->addChild(environment.background);

    // Shading
    Program *program = new Program;
    program->addShader(osgDB::readShaderFile(Shader::VERTEX, "Shaders/main.vert"));
    program->addShader(osgDB::readShaderFile(Shader::FRAGMENT, "Shaders/rpdt.frag"));
    program->addShader(osgDB::readShaderFile(Shader::FRAGMENT, "Shaders/normals.frag"));

    StateSet* state = hand->getOrCreateStateSet();
    state->setAttributeAndModes(program, StateAttribute::ON);

	// Interface
	NodeTrackerManipulator* manipulator = new NodeTrackerManipulator;
    manipulator->setNode(transform);

    Viewer viewer;
    viewer.setUpViewInWindow(200, 200, 1024, 1024);
    viewer.setCameraManipulator(manipulator);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.addEventHandler(new KeyboardHandler); 
    viewer.setSceneData(scene);
    viewer.realize();
    manipulator->setDistance(hand->getBound().radius() * 4);

    // Run
    fmat joints;
    joints << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0;
    joints = joints.t();
    pdt.rbfUpdate(joints);

    Timer timer;
    Timer_t last = 0;

    while (!viewer.done()) {
        //Quat(-1.5, Vec3(1, 0, 0))
        transform->setAttitude(Quat(Rotation, Vec3(0, 1, 0), -1.5, Vec3(1, 0, 0), 0, Vec3()));
        viewer.frame();
    }

    return viewer.run();
}