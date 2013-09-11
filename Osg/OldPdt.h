#include <list>
#include "Matrix.h"

struct Neighbor {
	Neighbor() {};
	Neighbor(float* c, float d) : coefs(c), distance(d) {};

	float* coefs;
	float distance;
};

struct Pdt {
	Pdt(const char* filename) {
		ifstream stream(filename);
		jointAverage = Matrixf(stream);
		jointBasis = Matrixf(stream);

		int numClusters;
		stream.read((char*) &numClusters, sizeof(int));

		clusterAverage = new Matrixf[numClusters];
		clusterBasis = new Matrixf[numClusters];

		for (int i = 0; i < numClusters; i++) {
			clusterAverage[i] = Matrixf(stream);
			clusterBasis[i] = Matrixf(stream);
		}

		clusters = Matrixi(stream);
		samples = Matrixf(stream);
	}

	void reduceJoints(float* joints) {
		//joints = jointBasis * (joints - jointAverage);
	}

	float* getNeighbor(float* reducedJoints) {
		float* neighbor;
		float minDistance;

		for (int i = 0; i < samples.area(); i += samples.width()) {
			float distance = 0;


			for (int c = i; c < jointBasis.height(); c++) {
				float off = reducedJoints[c] - samples[c];
				distance += off * off;
			}

			if (distance < minDistance) {
				neighbor = samples.values + i;
				distance = minDistance;
			}
		}

		return neighbor;
	}

	void getNeighbors(float* reducedJoints, int k) {
		list<Neighbor> neighbors(k);

		for (int i = 0; i < samples.area(); i += samples.width()) {
			float distance = 0;


			for (int c = i; c < jointBasis.height(); c++) {
				float off = reducedJoints[c] - samples[c];
				distance += off * off;
			}

			list<Neighbor>::iterator t;
			for (t = neighbors.begin(); t != neighbors.end(); t++)
				if (distance > (*t).distance)
					break;

			if (t != neighbors.begin()) {
				neighbors.insert(t, Neighbor(samples.values + i, distance));

				if (neighbors.size() > k)
					neighbors.pop_back();
			}
		}
	}

	Matrixf jointAverage, jointBasis, samples, *clusterAverage, *clusterBasis;
	Matrixi clusters;
};