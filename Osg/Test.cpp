#include <osg/PolygonMode>
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

#include "Classes/SceneController.h"
#include "Classes/EnvironmentSet.h"

using namespace osgShadow;
using namespace osgViewer;
using namespace osgDB;
using namespace osgGA;

Texture* createFloatTexture() {
    ref_ptr<Texture2D> texture = new Texture2D;
    texture->setTextureSize(1024, 1024);
    texture->setInternalFormat(GL_RGBA16F_ARB);
    texture->setSourceFormat(GL_RGBA);
    texture->setSourceType(GL_FLOAT);
    texture->setFilter(Texture2D::MIN_FILTER, Texture::LINEAR);
    texture->setFilter(Texture2D::MAG_FILTER, Texture::LINEAR);
    return texture.release();
}

Camera* createPreCam(Camera::BufferComponent buffer, Texture* tex) {
    ref_ptr<Camera> camera = new Camera;
    camera->setClearColor(Vec4());
    camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    camera->setRenderTargetImplementation(Camera::FRAME_BUFFER_OBJECT);
    camera->setRenderOrder(Camera::PRE_RENDER);

    if (tex) {
        camera->setViewport( 0, 0, tex->getTextureWidth(), tex->getTextureHeight() );
        camera->attach( buffer, tex );
    }
    
    return camera.release();
}

Camera* createPostCam(double left, double right, double bottom, double top) {
    ref_ptr<Camera> camera = new Camera;
    camera->setReferenceFrame( Transform::ABSOLUTE_RF );
    camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    camera->setRenderOrder( Camera::POST_RENDER );
    camera->setAllowEventFocus( false );
    camera->setProjectionMatrix( Matrix::ortho2D(left, right, bottom, top) );
    camera->getOrCreateStateSet()->setMode( GL_LIGHTING, StateAttribute::OFF );
    return camera.release();
}

Geode* createScreenQuad(float width, float height, float scale) {
    Geometry* geom = createTexturedQuadGeometry(
        Vec3(), Vec3(width,0.0f,0.0f), Vec3(0.0f,height,0.0f),
        0.0f, 0.0f, width*scale, height*scale );
    ref_ptr<Geode> quad = new Geode;
    quad->addDrawable( geom );
    
    int values = StateAttribute::OFF|StateAttribute::PROTECTED;
    quad->getOrCreateStateSet()->setAttribute(
        new PolygonMode(PolygonMode::FRONT_AND_BACK, PolygonMode::FILL), values );
    quad->getOrCreateStateSet()->setMode( GL_LIGHTING, values );
    return quad.release();
}

int main() {
	// Scene
	Group* scene = new ShadowedScene(new SoftShadowMap);
	Node *hand = osgDB::readNodeFile("../Captures/generated/300 poses/poses 100.0001.obj");

	PositionAttitudeTransform *transform = new PositionAttitudeTransform;
    transform->setPosition(Vec3(0, 0, -10));
    transform->addChild(hand);

	PdtState state("../Captures/generated/300 poses fine", transform);

	EnvironmentSet environments;
	environments.add("../Ambients/Museum.tga");
    environments.add("../Ambients/Street.tga");
    environments.add("../Ambients/Uffizi Gallery.tga");
	environments.show(0);

	fmat joints;
    joints << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0;
    joints = joints.t();
    state.updatePose(joints);
    state.updateScene(environments.ambient);

	scene->addChild(environments.source);
	scene->addChild(transform);

	// Main
	Texture *diffuse = createFloatTexture();
	Texture *specular = createFloatTexture();

	Camera *mainCam = createPreCam(Camera::COLOR_BUFFER0, specular);
    mainCam->attach(Camera::COLOR_BUFFER1, diffuse);
    mainCam->addChild(environments.background);
    mainCam->addChild(scene);

    Texture2D* normals = new Texture2D;
    normals->setImage(readImageFile("../Models/hand/normal.png"));
    Texture2D* specularity = new Texture2D;
    specularity->setImage(readImageFile("../Models/hand/specular.png"));

    Material* material = new Material;
    material->setSpecular(Material::FRONT, Vec4(.5, .5, .5, 1));
    material->setShininess(Material::FRONT, 4.0f);

    Program *mainProg = new Program;
    mainProg->addShader(readShaderFile(Shader::VERTEX, "Shaders/main.vert"));
    mainProg->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/shadow.frag"));
    mainProg->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/lights.frag"));
    mainProg->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/pdt.frag"));
    mainProg->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/main.frag"));

    StateSet* mainState = transform->getOrCreateStateSet();
    mainState->setAttributeAndModes(material, StateAttribute::MATERIAL);
    mainState->setAttributeAndModes(mainProg, StateAttribute::ON|StateAttribute::OVERRIDE);
    mainState->setTextureAttributeAndModes(0, normals);
    mainState->setTextureAttributeAndModes(3, specular);
    mainState->addUniform(new Uniform("normals", 0));
    mainState->addUniform(new Uniform("specularity", 3));

    // Final
	Camera *finalCam = createPostCam(0.0, 1.0, 0.0, 1.0);
    finalCam->addChild( createScreenQuad(0.5f, 1.0f, 1.0f) );

 	Program *finalProg = new Program;
 	finalProg->addShader(readShaderFile(Shader::VERTEX, "Shaders/ssss.vert"));
 	finalProg->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/ssss.frag"));

	StateSet* finalState = finalCam->getOrCreateStateSet();
	finalState->setAttributeAndModes(finalProg, StateAttribute::ON|StateAttribute::OVERRIDE);
    finalState->setTextureAttributeAndModes(0, diffuse);
    finalState->setTextureAttributeAndModes(1, specular);
    finalState->addUniform(new Uniform("diffuse", 0));
    finalState->addUniform(new Uniform("specular", 1));

    // Interface
    Group *root = new Group;
    root->addChild(mainCam);
    root->addChild(finalCam);
    root->addChild(scene);

    NodeTrackerManipulator *manipulator = new NodeTrackerManipulator;
    Viewer viewer;
    
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.addEventHandler(new SceneController(transform, &environments, &state)); 
    viewer.setCameraManipulator(manipulator);
    viewer.setSceneData(root);
    viewer.realize();
    manipulator->setDistance(80);

    return viewer.run();
}