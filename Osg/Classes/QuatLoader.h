#include <osg/NodeCallback>
#include <osg/NodeVisitor>
#include "../Math/Matrix.h"
#include "UpdateMethod.h"
using namespace osg;

class QuatLoader : public NodeCallback {
public:
	QuatLoader() : NodeCallback() {}
	QuatLoader(const char* path, UpdateMethod *_method) : NodeCallback(), method(_method), frame(0) {
		std::ifstream file(path, ios_base::binary);
   		readMatrix<float>(&frames, file);
	}

	void operator()(Node* node, NodeVisitor* nv ) {
		if (nv && nv->getVisitorType() == NodeVisitor::UPDATE_VISITOR) {
       		frame++;
       		if (frame == frames.n_cols)
       			frame = 0;

			cout << frame << endl;
       		method->updatePose(frames.col(frame));
		}

		traverse(node,nv);
    }

protected:
	int frame;
	fmat frames;
	UpdateMethod *method;
};