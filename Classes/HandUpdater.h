#include <VisualHandModel.cpp>
#include "PdtState.h"

struct HandUpdater : public UpdateMethod {
	HandUpdater(VisualHandModel *_hand, PdtState *_state) : hand(_hand), state(_state) {}

	void updatePose(fmat joints) {
		hand->setBoneRotation("lHand", Quat(joints[1], joints[2], joints[3], joints[0]));
		hand->setBoneRotation("lThumb1", Quat(joints[5], joints[6], joints[7], joints[4]));
		hand->setBoneRotation("lThumb2", Quat(joints[9], joints[10], joints[11], joints[8]));
		hand->setBoneRotation("lThumb3", Quat(joints[13], joints[14], joints[15], joints[12]));
		hand->setBoneRotation("lIndex1", Quat(joints[17], joints[18], joints[19], joints[16]));
		hand->setBoneRotation("lIndex2", Quat(joints[21], joints[22], joints[23], joints[20]));
		hand->setBoneRotation("lIndex3", Quat(joints[25], joints[26], joints[27], joints[24]));
		hand->setBoneRotation("lMid1", Quat(joints[29], joints[30], joints[31], joints[28]));
		hand->setBoneRotation("lMid2", Quat(joints[33], joints[34], joints[35], joints[32]));
		hand->setBoneRotation("lMid3", Quat(joints[37], joints[38], joints[39], joints[36]));
		hand->setBoneRotation("lRing1", Quat(joints[41], joints[42], joints[43], joints[40]));
		hand->setBoneRotation("lRing2", Quat(joints[45], joints[46], joints[47], joints[44]));
		hand->setBoneRotation("lRing3", Quat(joints[49], joints[50], joints[51], joints[48]));
		hand->setBoneRotation("lPinky1", Quat(joints[53], joints[54], joints[55], joints[52]));
		hand->setBoneRotation("lPinky2", Quat(joints[57], joints[58], joints[69], joints[56]));
		hand->setBoneRotation("lPinky3", Quat(joints[61], joints[62], joints[63], joints[60]));
		state->updatePose(joints);
	}

	VisualHandModel *hand;
	PdtState *state;
};