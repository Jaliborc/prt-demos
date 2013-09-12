#include "Pgt.h"
#include "Environment.h"
#include "FullscreenQuad.h"

#include <osg/Program>
#include <osg/Texture2D>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>

#define WIDTH 2048
#define HEIGHT 1024

using namespace osgShadow;
using namespace osgViewer;
using namespace osgGA;

Camera* NewPass(int order) {
    Camera* pass = new Camera;
    pass->setRenderTargetImplementation(Camera::FRAME_BUFFER_OBJECT);
    pass->setReferenceFrame(Transform::RELATIVE_RF);
    pass->setRenderOrder(Camera::PRE_RENDER, order);
    pass->setViewport(0,0, WIDTH, HEIGHT);
    return pass;
}

Texture2D* NewBuffer(GLint format = GL_RGBA, Texture::FilterMode filter = Texture::LINEAR) {
    Texture2D* buffer = new Texture2D;
    buffer->setTextureSize(WIDTH, HEIGHT);
    buffer->setInternalFormat(format);
    buffer->setFilter(Texture::MIN_FILTER, filter);
    buffer->setFilter(Texture::MAG_FILTER, filter);
    return buffer;
}

int main(int argc, char** argv) {
    // Scene
    ShadowedScene* scene = new ShadowedScene(new SoftShadowMap);
    Environment* light = new Environment("../Ambients/Museum.tga", scene);
    //Node* hand = readNodeFile("Media/hand.obj");
    Pgt* hand = new Pgt("Media/fingerBend.npgt");

    hand->render(0, light->ambient);
    scene->addChild(light->source);
    scene->addChild(hand);

    // Main Shading
    Texture2D* normals = new Texture2D;
    normals->setImage(readImageFile("Media/normal.png"));
    Texture2D* specular = new Texture2D;
    specular->setImage(readImageFile("Media/specular.png"));

    Material* material = new Material;
    material->setSpecular(Material::FRONT, Vec4(1,1,1,1));
    material->setShininess(Material::FRONT, 4.0f);

    Program* program = new Program;
    program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/shadow.frag"));
    program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/lights.frag"));
    program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/main.frag"));
    program->addShader(readShaderFile(Shader::VERTEX, "Shaders/main.vert"));

    StateSet* uniforms = hand->getOrCreateStateSet();
    uniforms->setAttributeAndModes(program, StateAttribute::ON);
    uniforms->setAttributeAndModes(material, StateAttribute::MATERIAL); 
    uniforms->setTextureAttributeAndModes(0, normals, StateAttribute::ON);
    uniforms->setTextureAttributeAndModes(3, specular, StateAttribute::ON);
    uniforms->addUniform(new Uniform("normals", 0));
    uniforms->addUniform(new Uniform("specular", 3));

    // Main Render
    Texture2D* diffuse = NewBuffer();
    Texture2D* reflected = NewBuffer();
    Texture2D* depth = NewBuffer(GL_DEPTH_COMPONENT, Texture::NEAREST);

    Camera* mainrender = NewPass(1);
    mainrender->attach(Camera::COLOR_BUFFER0, diffuse);
    mainrender->attach(Camera::COLOR_BUFFER1, reflected);
    mainrender->attach(Camera::DEPTH_BUFFER, depth);
    mainrender->addChild(light->background);
    mainrender->addChild(scene);

    //// SSSS Pass
    Node* display = new FullscreenQuad;
    program = new Program;
    program->addShader(readShaderFile(Shader::VERTEX, "Shaders/ssss.vert"));
    program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/ssss.frag"));

    uniforms = display->getOrCreateStateSet();
    uniforms->setAttributeAndModes(program, StateAttribute::ON);
    uniforms->setTextureAttributeAndModes(0, diffuse, StateAttribute::ON);
    uniforms->setTextureAttributeAndModes(1, depth, StateAttribute::ON);
    uniforms->setTextureAttributeAndModes(2, reflected, StateAttribute::ON);
    uniforms->addUniform(new Uniform("diffuse", 0));
    uniforms->addUniform(new Uniform("depth", 1));
    uniforms->addUniform(new Uniform("reflected", 2));

    // Interface
    Group* root = new Group;
    root->addChild(mainrender);
    root->addChild(display);

    NodeTrackerManipulator* manipulator = new NodeTrackerManipulator;
    manipulator->setNode(hand);

    Viewer viewer;
    viewer.setUpViewOnSingleScreen(1);
    viewer.setCameraManipulator(manipulator);
    viewer.getCamera()->setComputeNearFarMode(CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setSceneData(root);
    viewer.realize();
    manipulator->setDistance(hand->getBound().radius() * 4);

    // Run
    Timer timer;
    Timer_t last = timer.tick();
    int pose = 0;

    while (!viewer.done()) {
        Timer_t now = timer.tick();
        if (timer.delta_s(last, now) > 1.0/29.0) {
            last = now;
            hand->render(pose++, light->ambient);

            if (pose == hand->numPoses()) pose = 0;
        }

        viewer.frame();
    }

    return viewer.run();
}