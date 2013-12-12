#if defined(WIN32)
#include <windows.h>
#endif
#include "Classes/DefaultSetup.h"
#include "Classes/QuatLoader.h"

int main() {
    Node *hand = osgDB::readNodeFile("../Captures/generated/300 poses/poses 100.0001.obj");
    PositionAttitudeTransform *transform = new PositionAttitudeTransform;
    transform->setAttitude(Vec4(-1, 0, 0, 1));
    transform->addChild(hand);

    PdtState state("../Captures/generated/300 poses fine", hand);
    hand->setUpdateCallback(new QuatLoader("../Captures/raw/paper poses.quat", &state));
    
    return DefaultSetup(transform, state);
}