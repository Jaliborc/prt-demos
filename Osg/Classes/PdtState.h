#include "../Math/Rbf.h"
#include "Environment.h"
#include "Pdt.h"

#include <osg/StateSet>
#include <osg/Uniform>

float quatDistance(Quat a, Quat b) {
	float inner = a.x() * b.x() +
				a.y() * b.y() +
				a.z() * b.z() +
				a.w() * b.w();

	return 1 - inner*inner;
}

struct PdtState : Pdt {
	PdtState(const char* path, Node* mesh, Environment& scene) : Pdt(path) {
		radiance = new Uniform(Uniform::FLOAT_VEC3, "environmentTransfer", numMaps);
    	transfer = new Uniform(Uniform::FLOAT, "transferCoefs", sampleTransfer.n_cols);
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
		float minDistance = std::numeric_limits<float>::max();
		int neighbor;

		for (int i = 0; i < sampleJoints.n_cols; i++) {
			float distance = norm(joints - sampleJoints.col(i), 2);

			if (distance < minDistance) {
				minDistance = distance;
				neighbor = i;
			}
		}

		update(neighbor);
	}

	void rbfUpdate(fmat& joints) {
		reduceJoints(&joints);
		RunRBF(&joints, sampleJoints, sampleTransfer, 2);

		float* coefs = joints.memptr();
		transfer->setArray(new FloatArray(coefs, coefs + joints.n_elem));

		for (int c = 0; c < numMaps; c++)
			state->setTextureAttributeAndModes(c+4, getTexture(0, c), StateAttribute::ON);
	}

	void update(Quat* joints) {
		float minDistance = std::numeric_limits<float>::max();
		int neighbor;

		for (int i = 0; i < sampleJoints.n_cols; i++) {
			fvec sample = sampleJoints.col(i);

			float distance = 0;
			for (int j = 0, k = 0; j<16; j++) {
				distance += quatDistance(joints[j], Quat(sample(k+1), sample(k+2), sample(k+3), sample(k))) ;
				k += 4;
			}

			if (distance < minDistance) {
				minDistance = distance;
				neighbor = i;
			}
		}

		update(neighbor);
	}

	void update(int sample) {
		int i = clusters(sample);
		cout << sample << " (" << i << ")\n";

		if (i != cluster) {
			for (int c = 0; c < numMaps; c++)
			    state->setTextureAttributeAndModes(c+4, getTexture(i, c), StateAttribute::ON);

			cluster = i;
		}

		float* coefs = sampleTransfer.colptr(sample);
		transfer->setArray(new FloatArray(coefs, coefs + sampleTransfer.n_rows));
	}

	void reduceJoints(fmat* joints) {
		*joints = jointBasis * (*joints - jointAverage);
	}

	Uniform *transfer, *radiance;
	StateSet *state;
	int cluster;
};