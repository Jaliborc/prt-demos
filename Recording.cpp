#if defined(WIN32)
#include <windows.h>
#endif

#include "Classes/HandUpdater.h"
#include "Classes/PaperSetup.h"
#include "Classes/QuatLoader.h"

int main() {
	VisualHandModel model("../Models/hand/hand.fbx");
	Node* hand = model.getSkeleton();

    PdtState state("../Models/hand/1000 poses ld", hand);
	UpdateMethod *updater = new HandUpdater(&model, &state);
    hand->setUpdateCallback(new QuatLoader("../Captures/restrained paper poses.quat", updater));
   
    return PaperSetup(hand, state);
}