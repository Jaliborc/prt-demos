#if defined(WIN32)
#include <windows.h>
#endif
#include "Classes/DefaultSetup.h"

class HandUpdater : public NodeCallback {
public:
    HandUpdater() : NodeCallback() {}
    HandUpdater(PdtState *_state) : NodeCallback(), state(_state) {
        homing << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 1 << 0 << 0 << 0;
        homing = homing.t();

        bent << 0.999990294144347 << -2.66539379979022e-06 << -0.000610859385210209 << 0.00436330847064834 << 0.755239654390310 << -0.183159724875386 << -0.625373504659476 << -0.0705234662357334 << 0.991444861373810 << -0.130526192220052 << 0 << 0 << 0.999954760846545 << 0 << 0.00951190098399071 << 0 << 0.728666069634216 << 0.00221132912212462 << 0.00235276825788839 << 0.684861543283650 << 0.846611425145850 << 0 << 0 << 0.532211513228070 << 0.995732755247548 << 0 << 0 << 0.0922836937228170 << 0.630604794995919 << 0.00250591785846653 << 0.00203614111699122 << 0.776097395326897 << 0.842875286715198 << 0 << 0 << 0.538108958339083 << 0.998493888956387 << 0 << 0 << 0.0548630450918427 << 0.705620592180996 << 0.00228792100444414 << 0.00227835739933554 << 0.708582496534581 << 0.836716526367296 << 0 << 0 << 0.547636242869157 << 0.999726526946010 << 0 << 0 << 0.0233852799955268 << 0.995275256115439 << -0.00133346804936222 << -0.0138106963119697 << 0.0960970920199353 << 0.983555713801006 << 0 << 0 << 0.180604977366057 << 0.995915974681262 << 0 << 0 << 0.0902849454486854;
        bent = bent.t();

        step = 0.05;
        loop = 0;
    }

    void operator()(Node* node, NodeVisitor* nv ) {
       if (nv && nv->getVisitorType() == NodeVisitor::UPDATE_VISITOR)
            UpdatePose();
        traverse(node,nv);
    }

    void UpdatePose() {
       loop += step;

        if (loop > 1.0f) {
            step *= -1.0f;
            loop = 1.0f;
        } else if (loop < 0.0f) {
            step *= -1.0f;
            loop = 0.0f;
        }
        loop = 0.0f;

        fmat joints = homing * (1.0f - loop) + bent * loop;
        state->updatePose(joints);
    }

protected:
    float loop, step;
    fmat homing, bent;
    PdtState* state;
};

int main() {
    Node *hand = osgDB::readNodeFile("../Captures/generated/300 poses/poses 100.0001.obj");
    PositionAttitudeTransform *transform = new PositionAttitudeTransform;
    transform->setAttitude(Vec4(-1, 0, 0, 1));
    transform->addChild(hand);

    PdtState state("../Captures/generated/300 poses fine", hand);
    hand->setUpdateCallback(new HandUpdater(&state));
    
    return DefaultSetup(transform, state);
}