#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

class CNrpTimer : public IGUIAnimator
{
public:
	CNrpTimer( IGUIEnvironment* environment, IGUIElement* node, int time, core::stringc action );
	~CNrpTimer( void );

	virtual void draw();
private:
	int _time;
	int _startTime;
	bool _active;
	core::stringc _action;
};

}//namspace gui

}//namespace irr
