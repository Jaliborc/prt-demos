#include <osg/PositionAttitudeTransform>
#include "EnvironmentSet.h"
#include "PdtState.h"
using namespace osgGA;

class SceneController : public GUIEventHandler {
public:
    SceneController(PositionAttitudeTransform* t, EnvironmentSet* e, PdtState* s)
        : GUIEventHandler(), direction(0), rotation(0), last(0), scene(0), transform(t), environments(e), state(s) {
        environments->show(0);
    }

    void accept(GUIEventHandlerVisitor& v) {v.visit(*this);};
    bool handle(const GUIEventAdapter& event, GUIActionAdapter& _) {
        onInput(event);
        onFrame();
        return false;
    }

protected:
    void onInput(const GUIEventAdapter& event) {
        GUIEventAdapter::EventType type = event.getEventType();
        int key = event.getKey();

       if (type == GUIEventAdapter::KEYDOWN) {
            if (key == GUIEventAdapter::KEY_Left)
               direction = -0.1;
            else if (key ==  GUIEventAdapter::KEY_Right)
                direction = 0.1;
            else if (key == GUIEventAdapter::KEY_Up)
                environments->show(++scene);
            else if (key == GUIEventAdapter::KEY_Down)
                environments->show(--scene);

        } else if (type == GUIEventAdapter::KEYUP)
            if (key == GUIEventAdapter::KEY_Left || key == GUIEventAdapter::KEY_Right)
                direction = 0;
    }

    void onFrame() {
        Timer_t now = timer.tick();

        if (timer.delta_s(last, now) > 1.0/35.0) {
            rotation += direction;
            last = now;

            Matrixf correction = Matrixf::rotate(Vec3(0, 1, 0), Vec3(0, 0, 1));
            Matrixf rotate = Matrixf::rotate(-rotation, Vec3(0, 0, 1)) * correction;
            Vec3Array *radiance = rotateSH(rotate, environments->ambient);
            flipZonalSH(radiance);

            transform->setAttitude(Quat(rotation, Vec3(0, 0, 1)));
            //state->updateScene(radiance);
        }
    }

    Timer timer; Timer_t last;
    PositionAttitudeTransform * transform;
    EnvironmentSet* environments;
    PdtState* state;

    float direction, rotation;
    int scene;
};