#include "Environment.h"
#include "Pdt.h"
#include <osg/StateSet>
#include <osg/Uniform>

struct PdtState : Pdt {
	PdtState(const char* path, Node* mesh, Environment& scene) : Pdt(path) {
		radiance = new Uniform(Uniform::FLOAT_VEC3, "environmentTransfer", numMaps);
    	transfer = new Uniform(Uniform::FLOAT, "transferCoefs", sampleTransfer.n_rows);
		cluster = -1;

		state = mesh->getOrCreateStateSet();
		state->addUniform(new Uniform("transferMinima", minima));
		state->addUniform(radiance);
		state->addUniform(transfer);

		for (int i = 0; i < numMaps; i++) {
			ostringstream name;
			name << "transfer" << i;

			state->addUniform(new Uniform(name.str().c_str(), i+4));
			radiance->setElement(i, scene.ambient->at(i));
		}
	}

	void update(fmat& joints) {
		reduceJoints(&joints);
		update(closestNeighbor(joints));
	}

	void update(int sample) {
		int i = clusters(sample);
		if (i != cluster) {
			for (int c = 0; c < numMaps; c++)
			    state->setTextureAttributeAndModes(c+4, getTexture(i, c), StateAttribute::ON);

			cluster = i;
		}

		float* values = sampleTransfer.colptr(sample);
		transfer->setArray(new FloatArray(values, values + sampleTransfer.n_rows));
	}

	Uniform *transfer, *radiance;
	StateSet *state;
	int cluster;
};