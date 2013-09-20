#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>
#include "SceneController.h"
#include "MultipassEffects.h"

int DefaultSetup(Node *hand, PdtState& state) {
	// Scene
	Program* program = new Program;
	program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/pdt.frag")),
	program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/main.frag"));

    PositionAttitudeTransform *transform = new PositionAttitudeTransform;
    transform->setPosition(Vec3(0, 0, -10));
    transform->addChild(hand);

	EnvironmentSet environments;
    environments.add("../Ambients/Museum.tga");
    environments.add("../Ambients/Street.tga");
    environments.add("../Ambients/Uffizi Gallery.tga");
    environments.add("../Ambients/1 1.tga");
    environments.add("../Ambients/2 1.tga");
    environments.add("../Ambients/3 1.tga");

	Group *scene = MultipassEffects(transform, environments, program);

	// Interface
    NodeTrackerManipulator *manipulator = new NodeTrackerManipulator;
    osgViewer::Viewer viewer;
    
    viewer.setUpViewInWindow(200, 400, 1024, 1024);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.addEventHandler(new SceneController(transform, &environments, &state)); 
    viewer.setCameraManipulator(manipulator);
    viewer.setSceneData(scene);
    viewer.realize();
    manipulator->setDistance(80);

    return viewer.run();
}