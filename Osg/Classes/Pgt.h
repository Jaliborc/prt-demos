#include <osg/Geode>
#include <osgUtil/SmoothingVisitor>
#include <osgDB/ReadFile>
#include <fstream>

using namespace osgUtil;
using namespace osg;
using namespace std;

float* readFloats(ifstream& stream, int count) {
	float* values = new float[count];
	stream.read((char*) values, sizeof(float) * count);
	return values;
}

struct Svd {
	Svd(ifstream& stream) {
		 stream.read((char*) &size, sizeof(int) * 3);

		 m = readFloats(stream, size);
		 v = readFloats(stream, size * numCoefs);
		 u = readFloats(stream, numCoefs * numPoses);
	}

	void set(int pose) {
		coefs = u + pose * numCoefs;
	}

	float at(int i) {
		float value = m[i];

		for (int c = 0; c < numCoefs; c++)
			value += v[i + c*size] * coefs[c];

		return value;
	}

	int size, numCoefs, numPoses;
	float *m, *v, *u;
	float *coefs;
};

class Pgt : public Geode {
public:
	Pgt(const std::string& filename) : Geode() {
		ifstream stream(filename.c_str());

		points = new Svd(stream);
		transfer = new Svd(stream);

		vertices = new Vec3Array(points->size / 3);
		colors = new Vec4Array(points->size / 3);

		geometry = new Geometry;
		geometry->setUseDisplayList(false);
	    geometry->setVertexArray(vertices);
	    geometry->setColorArray(colors);
	    geometry->setColorBinding(Geometry::BIND_PER_VERTEX);
	    geometry->setDataVariance(Object::DYNAMIC);

		int facesSize;
		stream.read((char*) &facesSize, sizeof(int));

		int faces[facesSize];
		stream.read((char*) faces, sizeof(int) * facesSize);

		for (int i = 0; i < facesSize;) {
			DrawElementsUInt* face = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);
	    	face->push_back(faces[i++]);
	    	face->push_back(faces[i++]);
	    	face->push_back(faces[i++]);

	    	geometry->addPrimitiveSet(face);
		}

		addDrawable(geometry);
	}

	void render(int pose, Vec3Array* radiance) {
		points->set(pose);
		transfer->set(pose);

		for (int i = 0, p = 0, t = 0; i < vertices->size(); i++) {
			vertices->at(i) = Vec3(points->at(p++), points->at(p++), points->at(p++));
			colors->at(i) = Vec4(0,0,0, 1);

			for (int h = 0; h < numHarmonics(); h++)	{
				colors->at(i) += Vec4(
					transfer->at(t) * radiance->at(h).x(),
					transfer->at(t+1) * radiance->at(h).y(),
					transfer->at(t+2) * radiance->at(h).z(),
				0);

				t += 3;
			}
		}
		
		geometry->dirtyBound();
		accept(normalGenerator);
	}

	int numPoses() {
		return points->numPoses;
	}
	
	int numHarmonics() {
		return transfer->size / points->size;
	}

	Geometry *geometry;
	Vec3Array *vertices;
	Vec4Array *colors;

protected:
	Svd *points, *transfer;
	SmoothingVisitor normalGenerator;
};