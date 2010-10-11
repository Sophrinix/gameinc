#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

class CNrpTimer : public IGUIAnimator
{
public:
	CNrpTimer( IGUIEnvironment* environment, IGUIElement* node, size_t time, core::stringc action );
	~CNrpTimer( void );

	virtual void draw();
private:
	size_t _time;
	size_t _startTime;
	bool _active;
	core::stringc _action;
};

}//namspace gui

}//namespace irr
