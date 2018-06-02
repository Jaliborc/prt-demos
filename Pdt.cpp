#if defined(WIN32)
#include <windows.h>
#endif
#include "Classes/PaperSetup.h"
#include "Classes/QuatLoader.h"

int main() {
    Node *hand = osgDB::readNodeFile("../Captures/generated/300 poses/poses 100.0001.obj");
    PositionAttitudeTransform *transform = new PositionAttitudeTransform;
    transform->setAttitude(Vec4(-1, 0, 0, 1));
    transform->addChild(hand);

    PdtState state("../../50poses", hand);
    hand->setUpdateCallback(new QuatLoader("../Captures/restrained paper poses.quat", &state));
    
    return DefaultSetup(transform, state);
}