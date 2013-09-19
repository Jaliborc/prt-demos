#include <vector>
#include <osg/Group>
#include <osg/LightSource>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/Material>
#include <osgDB/ReadFile>

#include "../Math/Sh.h"

struct Environment {
	Environment(const string& imageFile) {
		string radianceFile = imageFile.substr(0, imageFile.find_last_of(".")) + ".radiance";
		std::ifstream stream(radianceFile.c_str());
		ambient = new Vec3Array;

		while(stream >> color.x() && stream >> color.y() && stream >> color.z())
			ambient->push_back(color);

		image = osgDB::readImageFile(imageFile);
		direction = dominantSHDirection(ambient);
		color = dominantSHColor(ambient, direction);
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