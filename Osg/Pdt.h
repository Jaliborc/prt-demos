#include <list>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

template<class type>
void readMatrix(Mat<type>* target, ifstream& source) {
	int size[2];
	source.read((char*) &size, sizeof(int) * 2);

	int total = size[0] * size[1];
	type content[total];
	source.read((char*) content, sizeof(type) * total);

	*target = Mat<type>(content, size[0], size[1]);
}

struct Pdt {
	Pdt(const char* filename) {
		ifstream stream(filename);

		readMatrix<float>(&jointAverage, stream);
		readMatrix<float>(&jointBasis, stream);
		readMatrix<float>(&sampleJoints, stream);

		int numClusters;
		stream.read((char*) &numClusters, sizeof(int));

		clusterAverage = new fmat[numClusters];
		clusterBasis = new fmat[numClusters];

		for (int i = 0; i < numClusters; i++) {
			readMatrix<float>(clusterAverage + i, stream);
			readMatrix<float>(clusterBasis + i, stream);
		}

		readMatrix<int>(&clusters, stream);
		readMatrix<float>(&sampleTransfer, stream);
	}

	fmat getTransfer(fmat& joints) {
		reduceJoints(&joints);
		return getTransfer(closestNeighbor(joints));
	}

	fmat getTransfer(int sample) {
		int i = clusters(sample);
		return clusterAverage[i] + clusterBasis[i] * sampleTransfer.col(sample);
	}

	void reduceJoints(fmat* joints) {
		*joints = jointBasis * (*joints - jointAverage);
	}

	int closestNeighbor(fmat& reducedJoints) {
		float minDistance = numeric_limits<int>::max();
		int neighbor;

		for (int i = 0; i < sampleJoints.n_cols; i++) {
			fvec sample = sampleJoints.col(i);
			float distance = norm(sample - reducedJoints, 1);

			if (distance < minDistance) {
				minDistance = distance;
				neighbor = i;
			}
		}

		return neighbor;
	}

	fmat jointAverage, jointBasis, sampleJoints, *clusterAverage, *clusterBasis, sampleTransfer;
	imat clusters;
};