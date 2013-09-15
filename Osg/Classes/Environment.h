#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H 1

#include "../Math/Sh.h"
#include <osg/Group>
#include <osg/LightSource>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/Material>
#include <osgDB/ReadFile>

struct Environment {
	Environment(const string& textureFile) : color() {
		string radianceFile = textureFile.substr(0, textureFile.find_last_of(".")) + ".radiance";
		std::ifstream stream(radianceFile.c_str());
		ambient = new Vec3Array;

		while(stream >> color.x() && stream >> color.y() && stream >> color.z())
			ambient->push_back(color);

		direction = dominantSHDirection(ambient);
		color = dominantSHColor(ambient, direction);

		Light* light = new Light;
	    light->setLightNum(0);
	    light->setPosition(Vec4(direction, 0));
	    light->setDiffuse(Vec4(color, 1));
	   	light->setSpecular(Vec4(1,1,1, 1));
	    light->setAmbient(Vec4());

	    source = new LightSource;
	    source->setLight(light);

	    background = new Geode();
		background->addDrawable(new ShapeDrawable(new Sphere(Vec3(0, 10, 0), 300)));

		Texture2D* texture = new Texture2D;
		texture->setImage(osgDB::readImageFile(textureFile));

		StateSet* state = background->getOrCreateStateSet();
		state->setAttribute(new Material);
		state->setTextureAttributeAndModes(0, texture, StateAttribute::ON);
		state->setMode(GL_LIGHTING, StateAttribute::OFF);
	}

	LightSource* source;
	Geode* background;
	
	Vec3 direction, color;
	Vec3Array *ambient;
};

#endif