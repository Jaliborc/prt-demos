#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>

#include "EnvironmentSet.h"
#include "FullscreenQuad.h"

#define BUFFER_WIDTH 2048
#define BUFFER_HEIGHT 1024

using namespace osgDB;
using namespace osgShadow;

Camera* NewPass(int order) {
    Camera* pass = new Camera;
    pass->setRenderTargetImplementation(Camera::FRAME_BUFFER_OBJECT);
    pass->setReferenceFrame(Transform::RELATIVE_RF);
    pass->setRenderOrder(Camera::PRE_RENDER, order);
    pass->setViewport(0,0, BUFFER_WIDTH, BUFFER_HEIGHT);
    return pass;
}

Texture2D* NewBuffer(GLint format = GL_RGBA, Texture::FilterMode filter = Texture::LINEAR) {
    Texture2D* buffer = new Texture2D;
    buffer->setInternalFormat(format);
    buffer->setTextureSize(BUFFER_WIDTH, BUFFER_HEIGHT);
    buffer->setFilter(Texture::MIN_FILTER, filter);
    buffer->setFilter(Texture::MAG_FILTER, filter);
    return buffer;
}

Group* MultipassEffects(Node* mesh, EnvironmentSet& environments, Program* program) {
	// Scene
	Group* root = new Group;
	ShadowedScene* scene = new ShadowedScene(new SoftShadowMap);
	scene->addChild(environments.source);
	scene->addChild(mesh);

	// Main Shading
    Texture2D* normals = new Texture2D;
    normals->setImage(readImageFile("../Models/hand/normal.png"));
    Texture2D* specular = new Texture2D;
    specular->setImage(readImageFile("../Models/hand/specular.png"));

    Material* material = new Material;
    material->setSpecular(Material::FRONT, Vec4(.5, .5, .5, 1));
    material->setShininess(Material::FRONT, 4.0f);

    program->addShader(readShaderFile(Shader::VERTEX, "Shaders/main.vert"));
    program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/shadow.frag"));
    program->addShader(readShaderFile(Shader::FRAGMENT, "Shaders/lights.frag"));

    StateSet* uniforms = mesh->getOrCreateStateSet();
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

    Camera* main = NewPass(1);
    main->attach(Camera::COLOR_BUFFER0, diffuse);
    main->attach(Camera::COLOR_BUFFER1, reflected);
    main->attach(Camera::DEPTH_BUFFER, depth);
    main->addChild(environments.background);
    main->addChild(scene);

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

    // Final
    root->addChild(main);
	root->addChild(display);
	return root;
}