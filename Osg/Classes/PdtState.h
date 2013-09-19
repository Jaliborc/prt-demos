#include <osg/StateSet>
#include <osg/Uniform>

#include "../Math/Rbf.h"
#include "Pdt.h"

struct PdtState : Pdt {
	PdtState(const char* path, Node* mesh) : Pdt(path) {
		radiance = new Uniform(Uniform::FLOAT_VEC3, "environmentTransfer", numSH);
    	transfer = new Uniform(Uniform::FLOAT, "transferCoefs", weights.n_cols);
		cluster = -1;

		state = mesh->getOrCreateStateSet();
		state->addUniform(new Uniform("transferMinima", minima));
		state->addUniform(radiance);
		state->addUniform(transfer);

		for (int i = 0; i < numSH; i++) {
			ostringstream name;
			name << "transfer" << i;

			state->addUniform(new Uniform(name.str().c_str(), i+4));
			state->setTextureAttributeAndModes(i+4, getTexture(0, i), StateAttribute::ON);
		}
	}

	void updateScene(Vec3Array* ambient) {
		for (int i = 0; i < numSH; i++)
			radiance->setElement(i, ambient->at(i));
	}

	void updatePose(fmat& joints) {
		reducePose(&joints);
		RunRBF(&joints, jointSamples, weights, 2);

		float* coefs = joints.memptr();
		transfer->setArray(new FloatArray(coefs, coefs + joints.n_elem));
	}

	void reducePose(fmat* joints) {
		*joints = jointBasis * (*joints - jointAverage);
	}

	Uniform *transfer, *radiance;
	StateSet *state;
	int cluster;
};