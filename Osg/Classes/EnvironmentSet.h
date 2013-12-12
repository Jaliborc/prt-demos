#ifndef __ENVIRONMENT_SET
#define __ENVIRONMENT_SET

#include <vector>
#include <osg/Group>
#include <osg/LightSource>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/Material>
#include <osgDB/ReadFile>

#include "../Math/Sh.h"
#define Environment_Correction Matrixf::rotate(Vec3(0, 1, 0), Vec3(0, 0, 1))

struct Environment {
	Environment(const string& path) {
		ambient = new Vec3Array;
		image = osgDB::readImageFile(path);

		string radiancePath = path.substr(0, path.find_last_of(".")) + ".radiance";
		std::ifstream stream(radiancePath.c_str());
		string line;

		if (getline(stream, line)) {
			istringstream parser(line);

			while(parser >> color.x() && parser >> color.y() && parser >> color.z())
				ambient->push_back(color);

			if (getline(stream, line)) {
				istringstream parser(line);
				parser >> direction.x();
				parser >> direction.y();
				parser >> direction.z();
			} else
				direction = dominantSHDirection(ambient);

			color = dominantSHColor(ambient, direction);
			direction = Matrixf::transform3x3(direction, Environment_Correction);
		}
	}

	ref_ptr<Image> image;
	Vec3 direction, color;
	Vec3Array *ambient;
};

struct EnvironmentSet {
	EnvironmentSet() {
		light = new Light;
	    light->setLightNum(0);
	   	light->setSpecular(Vec4(1,1,1, 1));
	    light->setAmbient(Vec4());

	    source = new LightSource;
	    source->setLight(light);

	    background = new Geode();
		background->addDrawable(new ShapeDrawable(new Sphere(Vec3(0, 10, 0), 300)));
		texture = new Texture2D;

		StateSet* state = background->getOrCreateStateSet();
		state->setAttribute(new Material);
		state->setTextureAttributeAndModes(0, texture, StateAttribute::ON);
		state->setMode(GL_DEPTH_TEST, StateAttribute::OFF); 
		state->setMode(GL_LIGHTING, StateAttribute::OFF);
	}

	void add(const string& file) {
		scenes.push_back(new Environment(file));
	}

	void show(int i) {
		Environment *scene = scenes[i % scenes.size()];
		light->setPosition(Vec4(scene->direction, 0));
	    light->setDiffuse(Vec4(scene->color, 1));
	    texture->setImage(scene->image.get());
	    
	    ambient = scene->ambient;
	}

	Vec3Array *ambient;
	LightSource *source;
	Geode *background;

protected:
	vector<Environment*> scenes;
	Texture2D *texture;
	Light* light;
};

#endif