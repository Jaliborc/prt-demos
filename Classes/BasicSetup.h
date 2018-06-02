#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

using namespace osgViewer;
using namespace osgDB;
using namespace osgGA;
using namespace osg;

int BasicSetup(Node* hand) {
	NodeTrackerManipulator *manipulator = new NodeTrackerManipulator;
    Viewer viewer;
    
	viewer.setUpViewInWindow(50, 50, 720, 480, 0);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setCameraManipulator(manipulator);
    viewer.setSceneData(hand);
    viewer.realize();
    manipulator->setDistance(80);

    return viewer.run();
}