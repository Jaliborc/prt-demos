#include "Slider.h"
#include <osgWidget/Canvas>
#include <osgWidget/Label>
#include <osgWidget/WindowManager>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/ViewerEventHandlers>
#include <osgViewer/Viewer>

using namespace osg;
using namespace osgWidget;
using namespace osgViewer;

int main() {
	float value = 5.3f;
	float value2 = 8.0f;

	Viewer viewer;
	WindowManager* manager = new WindowManager(&viewer, 1280.0f, 720.0f, 0xF0000000);
	
	manager->addChild(new Slider("Test:%.2f", &value, 0.04f, Vec2(1000, 640)));
	manager->addChild(new Slider("Test2:%.2f", &value2, 1, Vec2(1000, 540)));

	viewer.setUpViewInWindow(
        50,
        50,
        static_cast<int>(manager->getWidth()),
        static_cast<int>(manager->getHeight())
    );

    Camera* camera = manager->createParentOrthoCamera();

    viewer.addEventHandler(new MouseHandler(manager));
    viewer.addEventHandler(new KeyboardHandler(manager));
    viewer.addEventHandler(new ResizeHandler(manager, camera));
    viewer.addEventHandler(new CameraSwitchHandler(manager, camera));
    viewer.addEventHandler(new WindowSizeHandler());

    manager->resizeAllWindows();
    viewer.setSceneData(camera);

    return viewer.run();
}