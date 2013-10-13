#include <osg/PositionAttitudeTransform>
#include "EnvironmentSet.h"
#include "PdtState.h"
using namespace osgGA;

class SceneController : public GUIEventHandler {
public:
    SceneController(PositionAttitudeTransform* t, EnvironmentSet* e, PdtState* s, Material* m)
        : GUIEventHandler(), direction(0), rotation(0), last(0), scene(0), transform(t), environments(e), state(s), material(m) {
        environments->show(0);
        specular = 0.25;
        diffuse = 0.65;
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
            else if (key == GUIEventAdapter::KEY_Q)
                specular += 0.005;
            else if (key == GUIEventAdapter::KEY_A)
                specular -= 0.005;
            else if (key == GUIEventAdapter::KEY_W)
                diffuse += 0.05;
            else if (key == GUIEventAdapter::KEY_S)
                diffuse -= 0.05;

        } else if (type == GUIEventAdapter::KEYUP)
            if (key == GUIEventAdapter::KEY_Left || key == GUIEventAdapter::KEY_Right)
                direction = 0;

        cout << "Specular: " << specular << ", Diffuse: " << diffuse << "\n";
    }

    void onFrame() {
        Timer_t now = timer.tick();

        if (timer.delta_s(last, now) > 1.0/35.0) {
            rotation += direction;
            last = now;

            Matrixf rotate = Matrixf::rotate(-rotation, Vec3(0, 0, 1)) * Environment_Correction;
            Vec3Array *radiance = rotateSH(rotate, environments->ambient);
            flipZonalSH(radiance);

            transform->setAttitude(Quat(rotation, Vec3(0, 0, 1)));
            state->updateScene(radiance);

            material->setSpecular(Material::FRONT, Vec4(specular, specular, specular, 1));
            material->setDiffuse(Material::FRONT, Vec4(diffuse, diffuse, diffuse, 1));
        }
    }

    Timer timer; Timer_t last;
    PositionAttitudeTransform * transform;
    EnvironmentSet* environments;
    Material* material;
    PdtState* state;

    float direction, rotation;
    float specular, diffuse;
    int scene;
};