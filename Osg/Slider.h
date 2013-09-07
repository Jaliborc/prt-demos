#include <osgWidget/Box>
#include <osgWidget/Label>

using namespace osg;
using namespace osgWidget;

class Slider : public osgWidget::Box {
public:
	Slider(const char* _format, float* value, float step, Vec2 point) : osgWidget::Box("", Box::HORIZONTAL), format(_format), target(value), off(step) {
		newButton("+", new Callback(&Slider::onPlus, this, EVENT_MOUSE_PUSH));
		label = newLabel();
		newButton("-", new Callback(&Slider::onMinus, this, EVENT_MOUSE_PUSH));

        getBackground()->setColor(0, 0, 0, 0); 
		setOrigin(point);
		updateLabel();
	}

	bool onMinus(Event& event) {
		(*target) -= off;
		updateLabel();
	}

	bool onPlus(Event& event) {
		(*target) += off;
		updateLabel();
	}

	void updateLabel() {
		char text[50];
		sprintf(text, format, *target);
		label->setLabel(text);
	}

	float *target, off;
	const char* format;

protected:
	Label* newButton(const char* text, Callback* callback) {
		Label* button = newLabel();
		button->setEventMask(EVENT_MOUSE_PUSH);
        button->addCallback(callback);
        button->setLabel(text);

        return button;
	}

	Label* newLabel() {
		Label* label = new Label;
		label->setFont("fonts/Vera.ttf");
		label->setFontColor(0, 0, 0, 1.0f);
		label->setColor(0, 0, 0, .3f);
    	label->setFontSize(15);

    	addWidget(label);
    	return label;
	}

	Label* label;
};