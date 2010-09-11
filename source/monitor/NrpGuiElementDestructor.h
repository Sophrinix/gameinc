#pragma once
#include <irrlicht.h>

namespace irr
{

namespace gui
{

class CNrpGuiElementDestructor : public IGUIElement
{
public:
	CNrpGuiElementDestructor( IGUIEnvironment* environment, IGUIElement* node, int time );
	~CNrpGuiElementDestructor(void);

	virtual void draw();
private:
	int _delay;
	int _startTime;
};

}//namspace gui

}//namespace irr
