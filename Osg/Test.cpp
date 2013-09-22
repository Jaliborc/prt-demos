#include "Classes/DefaultSetup.h"
using namespace osgViewer;

Group* makeScene() {
	//ShadowedScene* scene = new ShadowedScene(new SoftShadowMap);
	Group* scene = new Group;
	Node *hand = osgDB::readNodeFile("../Captures/generated/300 poses/poses 100.0001.obj");
	EnvironmentSet environments;
	environments.add("../Ambients/2 1.tga");
	environments.add("../Ambients/3 1.tga");
	environments.show(0);

	scene->addChild(environments.source);
	scene->addChild(hand);

	return scene;

}

int main() {
	Group *scene = makeScene();

	Texture2D* tex = new Texture2D;
	tex->setTextureWidth(800);
	tex->setTextureHeight(600);
	tex->setInternalFormat(GL_RGBA);
	tex->setFilter(Texture::MIN_FILTER, Texture::NEAREST); 
	tex->setFilter(Texture::MAG_FILTER, Texture::NEAREST); 

	Camera* pre = new Camera; 
	pre->setRenderTargetImplementation(Camera::FRAME_BUFFER_OBJECT); 
	pre->attach(Camera::COLOR_BUFFER0, tex, 0, 0, false, 8, 8);
	pre->setRenderOrder(Camera::PRE_RENDER);
    pre->setViewport(0,0, 800, 600);
	pre->addChild(scene);

	Node* quad = new FullscreenQuad; 
	quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex, StateAttribute::ON); 

	Group* root = new Group;
	root->addChild(pre);
	root->addChild(quad);
	
	Viewer viewer;
    viewer.setUpViewInWindow(200, 400, 1024, 1024);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setSceneData(root);
    viewer.realize();

    return viewer.run();
}