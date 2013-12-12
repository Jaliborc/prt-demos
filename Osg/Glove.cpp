#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include <ShapeHandFrame.h>
#include <ShapeHandClient.h>

#include "Classes/HandUpdater.h"
#include "Classes/PaperSetup.h"
#include "Classes/QuatLoader.h"

class GloveListener : public NodeCallback {
public:
    GloveListener() : NodeCallback() {}
    GloveListener(ShapeHandClient *_client, UpdateMethod *_method) : NodeCallback(), client(_client), method(_method) {}

    void operator()(Node* node, NodeVisitor* nv ) {
       if (nv && nv->getVisitorType() == NodeVisitor::UPDATE_VISITOR) {
            client->requestDataStream();
            client->getFrame(frame);

			frame.finger[0][0][1] *= -1;
			frame.hand[2] *= -1;

			for (int i=1; i<5; i++)
				frame.finger[i][0][2] *= -1;

			fmat joints(frame.hand, 64, 1);
			for (int i=0; i < 64; i+=4)
				if (joints[i] < 0) {
					joints[i] *= -1;
					joints[i+1] *= -1;
					joints[i+2] *= -1;
					joints[i+3] *= -1;
				}

			method->updatePose(joints);	
        }

        traverse(node,nv);
    }

protected:
    ShapeHandClient *client;
    ShapeHandFrame frame;
	UpdateMethod *method;
};

int main() {
	VisualHandModel model("../Models/hand/hand.fbx");
	Node* hand = model.getSkeleton();

    PdtState state("../Captures/generated/300 poses fine", hand);
	UpdateMethod *updater = new HandUpdater(&model, &state);
	ShapeHandClient *client = new ShapeHandClient(10981);

	if (client->connect())
        hand->setUpdateCallback(new GloveListener(client, updater));
    
    return PaperSetup(hand, state);
}