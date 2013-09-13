#include "Mpdt.h"
#include <osg/StateSet>
#include <osg/Uniform>

struct PdtShader : Pdt {
	PdtShader(const char* path, StateSet* uniforms) : Pdt(path), state(uniforms) {
		for (int c = 0; c < numMaps; c++) {
			ostringstream name;
			name << "transfer" << c;
			state->addUniform(new Uniform(name.str().c_str(), c+3));
		}

		cluster = -1;
		transfer = new Uniform(Uniform::FLOAT, "transferCoefs", sampleTransfer.n_rows);
		state->addUniform(transfer);
	}

	void update(fmat& joints) {
		reduceJoints(&joints);
		update(closestNeighbor(joints));
	}

	void update(int sample) {
		int i = clusters(sample);
		if (i != cluster) {
			for (int c = 0; c < numMaps; c++)
			    state->setTextureAttributeAndModes(c+3, getTexture(i, c), StateAttribute::ON);

			cluster = i;
		}

		float* values = sampleTransfer.colptr(sample);
		transfer->setArray(new FloatArray(values, values + sampleTransfer.n_rows));
	}

	Uniform* transfer;
	StateSet* state;
	int cluster;
};