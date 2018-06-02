#include <limits>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include "../Math/Matrix.h"
using namespace osg;

struct Pdt {
	Pdt(const char* path) {
		ostringstream filename;
		filename << path << ".pdt";
		ifstream stream(filename.str().c_str(), ios_base::binary);

		readMatrix<float>(&jointAverage, stream);
		readMatrix<float>(&jointBasis, stream);
		readMatrix<float>(&jointSamples, stream);
		readMatrix<float>(&weights, stream);
		readMatrix<int>(&clusters, stream);
		stream.read((char*) &numClusters, sizeof(int) * 3);

		numMaps = numClusters * numSH;
		numScalars = numMaps * (weights.n_cols+1) * 2;
		textures = new ref_ptr<Texture2D>[numMaps];
		scalars = new float[numScalars];

		for (int i = 0; i < numClusters; i++)
			for (int c = 0; c < numSH; c++) {
				ostringstream name;
				name << path << i << "_" << c << ".jpg";

				int index = i * numSH + c;
				textures[index] = new Texture2D;
				textures[index]->setImage(osgDB::readImageFile(name.str().c_str()));
			}

		stream.read((char*) scalars, sizeof(float) * numScalars);
	}

	ref_ptr<Texture2D> getTexture(int cluster, int map) {
		return textures[cluster * numSH + map];
	}

	ref_ptr<Texture2D> *textures;
	fmat jointAverage, jointBasis, jointSamples, weights;
	imat clusters;

	int numClusters, numSH, numMaps, numScalars;
	float *scalars;
};