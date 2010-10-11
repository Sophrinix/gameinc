#pragma once
#include <irrlicht.h>

namespace irr
{

namespace gui
{

class CNrpGuiElementDestructor : public IGUIElement
{
public:
	CNrpGuiElementDestructor( IGUIEnvironment* environment, IGUIElement* node, size_t time );
	~CNrpGuiElementDestructor(void);

	virtual void draw();
private:
	size_t _delay;
	size_t _startTime;
};

}//namspace gui

}//namespace irr
