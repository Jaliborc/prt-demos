#include <sstream>
#include <fstream>
#include <armadillo>
#include <limits>
#include <osgDB/ReadFile>
#include <osg/Texture2D>

using namespace std;
using namespace arma;
using namespace osg;

template<class type>
void readMatrix(Mat<type>* target, ifstream& source) {
	int size[2];
	source.read((char*) &size, sizeof(int) * 2);

	int total = size[0] * size[1];
	type* content = new type[total];
	source.read((char*) content, sizeof(type) * total);

	*target = Mat<type>(content, size[0], size[1]);
	delete content;
}

struct Pdt {
	Pdt(const char* path) {
		ostringstream filename;
		filename << path << ".pdt";
		ifstream stream(filename.str().c_str(), ios_base::binary);

		readMatrix<float>(&jointAverage, stream);
		readMatrix<float>(&jointBasis, stream);
		readMatrix<float>(&sampleJoints, stream);
		readMatrix<float>(&sampleTransfer, stream);
		readMatrix<int>(&clusters, stream);

		stream.read((char*) &minima, sizeof(float));
		stream.read((char*) &numClusters, sizeof(int) * 2);
		textures = new ref_ptr<Texture2D>[numClusters * numMaps];

		for (int i = 0; i < numClusters; i++)
			for (int c = 0; c < numMaps; c++) {
				ostringstream name;
				name << path << i << "_" << c << ".png";

				int index = i * numMaps + c;
				textures[index] = new Texture2D;
				textures[index]->setImage(osgDB::readImageFile(name.str().c_str()));
			}
	}

	void reduceJoints(fmat* joints) {
		*joints = jointBasis * (*joints - jointAverage);
	}

	int closestNeighbor(fmat& reducedJoints) {
		float minDistance = std::numeric_limits<float>::max();
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

	ref_ptr<Texture2D> getTexture(int cluster, int map) {
		return textures[cluster * numMaps + map];
	}

	ref_ptr<Texture2D> *textures;
	fmat jointAverage, jointBasis, sampleJoints, sampleTransfer;
	imat clusters;

	float minima;
	int numClusters, numMaps;
};