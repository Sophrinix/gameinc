#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiTimeDestructor : public IGUIAnimator
{
public:
	CNrpGuiTimeDestructor( IGUIEnvironment* environment, IGUIElement* node, int time );
	~CNrpGuiTimeDestructor( void );

	virtual void draw();
private:
	int _time;
	int _startTime;
	bool _active;
};

}//namspace gui

}//namespace irr
