#include <winsock2.h>
#include <windows.h>

#include <osg/TexEnv>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>

#include <VisualHandModel.cpp>
#include <ShapeHandFrame.h>
#include <ShapeHandClient.h>

#include "Classes/DefaultSetup.h"

using namespace osgViewer;
using namespace osgGA;
using namespace osg;
using namespace std;

class HandUpdater : public NodeCallback {
public:
    HandUpdater() : NodeCallback() {}
    HandUpdater(ShapeHandClient *_client, VisualHandModel *_hand, PdtState* _state) : NodeCallback(), hand(_hand), client(_client), state(_state) {}

    void operator()(Node* node, NodeVisitor* nv ) {
       if (nv && nv->getVisitorType() == NodeVisitor::UPDATE_VISITOR) {
            client->requestDataStream();
            client->getFrame(joints);

			joints.finger[0][0][1] *= -1;
			joints.hand[2] *= -1;

			for (int i=1; i<5; i++)
				joints.finger[i][0][2] *= -1;

            UpdateSkeleton();
			UpdateColor();
        }

        traverse(node,nv);
    }

	void UpdateSkeleton() {
		hand->setBoneRotation("lHand", Quat(joints.hand[1], joints.hand[2], joints.hand[3], joints.hand[0]));
		hand->setBoneRotation("lThumb1", Quat(joints.finger[0][0][1], joints.finger[0][0][2], joints.finger[0][0][3], joints.finger[0][0][0]));
		hand->setBoneRotation("lThumb2", Quat(joints.finger[0][1][1], joints.finger[0][1][2], joints.finger[0][1][3], joints.finger[0][1][0]));
		hand->setBoneRotation("lThumb3", Quat(joints.finger[0][2][1], joints.finger[0][2][2], joints.finger[0][2][3], joints.finger[0][2][0]));
		hand->setBoneRotation("lIndex1", Quat(joints.finger[1][0][1],  joints.finger[1][0][2], joints.finger[1][0][3], joints.finger[1][0][0]));
		hand->setBoneRotation("lIndex2", Quat(joints.finger[1][1][1],  joints.finger[1][1][2], joints.finger[1][1][3], joints.finger[1][1][0]));
		hand->setBoneRotation("lIndex3", Quat(joints.finger[1][2][1], joints.finger[1][2][2],  joints.finger[1][2][3], joints.finger[1][2][0]));
		hand->setBoneRotation("lMid1", Quat(joints.finger[2][0][1],  joints.finger[2][0][2], joints.finger[2][0][3], joints.finger[2][0][0]));
		hand->setBoneRotation("lMid2", Quat(joints.finger[2][1][1],  joints.finger[2][1][2], joints.finger[2][1][3], joints.finger[2][1][0]));
		hand->setBoneRotation("lMid3", Quat(joints.finger[2][2][1], joints.finger[2][2][2], joints.finger[2][2][3], joints.finger[2][2][0]));
		hand->setBoneRotation("lRing1", Quat(joints.finger[3][0][1] , joints.finger[3][0][2], joints.finger[3][0][3], joints.finger[3][0][0]));
		hand->setBoneRotation("lRing2", Quat(joints.finger[3][1][1], joints.finger[3][1][2], joints.finger[3][1][3], joints.finger[3][1][0]));
		hand->setBoneRotation("lRing3", Quat(joints.finger[3][2][1], joints.finger[3][2][2], joints.finger[3][2][3], joints.finger[3][2][0]));
		hand->setBoneRotation("lPinky1", Quat(joints.finger[4][0][1], joints.finger[4][0][2], joints.finger[4][0][3], joints.finger[4][0][0]));
		hand->setBoneRotation("lPinky2", Quat(joints.finger[4][1][1], joints.finger[4][1][2], joints.finger[4][1][3], joints.finger[4][1][0]));
		hand->setBoneRotation("lPinky3", Quat(joints.finger[4][2][1], joints.finger[4][2][2], joints.finger[4][2][3], joints.finger[4][2][0]));
	}

	void UpdateColor() {
		fmat quats(joints.hand, 64, 1);

		for (int i=0; i < 64; i+=4)
			if (quats(i) < 0) {
				quats(i) *= -1;
				quats(i+1) *= -1;
				quats(i+2) *= -1;
				quats(i+3) *= -1;
			}

		state->updatePose(quats);
	}

protected:
    VisualHandModel *hand;
    ShapeHandClient *client;
    ShapeHandFrame joints;
	PdtState* state;
};

int main() {
	VisualHandModel hand("../Models/hand/hand.fbx");
	Node* mesh = hand.getSkeleton();
	PdtState state("../Captures/generated/300 poses fine", mesh);

	ShapeHandClient *handClient = new ShapeHandClient(10981);
    if (handClient->connect())
        mesh->setUpdateCallback(new HandUpdater(handClient, &hand, &state));

    return DefaultSetup(mesh, state);
}