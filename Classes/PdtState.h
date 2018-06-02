#ifndef PDT_STATE_H
#define PDT_STATE_H

#include <osg/StateSet>
#include <osg/Uniform>

#include "../Math/Rbf.h"
#include "UpdateMethod.h"
#include "Pdt.h"

struct PdtState : Pdt, UpdateMethod {
	PdtState(const char* path, Node* mesh) : Pdt(path) {
		Uniform *scale = new Uniform(Uniform::FLOAT, "transferScalars", numScalars);
		scale->setArray(new FloatArray(scalars, scalars + numScalars));

		radiance = new Uniform(Uniform::FLOAT_VEC3, "environmentTransfer", numSH);
    	transfer = new Uniform(Uniform::FLOAT, "transferCoefs", weights.n_cols+1);

		state = mesh->getOrCreateStateSet();
		state->addUniform(scale);
		state->addUniform(radiance);
		state->addUniform(transfer);

		for (int i = 0; i < numSH; i++) {
			ostringstream name;
			name << "transfer" << i;

			state->addUniform(new Uniform(name.str().c_str(), i+4));
			state->setTextureAttributeAndModes(i+4, getTexture(0, i).get(), StateAttribute::ON);
		}
	}

	void updateScene(Vec3Array* ambient) {
		for (int i = 0; i < numSH; i++)
			radiance->setElement(i, ambient->at(i));
	}

	void updatePose(fmat joints) {
		reducePose(&joints);
		RunRBF(&joints, jointSamples, weights, 2);

		FloatArray* coefs = new FloatArray(joints.memptr() - 1, joints.memptr() + joints.n_elem);
		coefs->at(0) = 1;
		transfer->setArray(coefs);
	}

	void reducePose(fmat* joints) {
		*joints = jointBasis * (*joints - jointAverage);
	}

	Uniform *transfer, *radiance;
	StateSet *state;
};

#endif