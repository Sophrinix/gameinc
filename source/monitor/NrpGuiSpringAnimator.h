#ifndef _INCLUDE_SPRINGANIMATOR_
#define _INCLUDE_SPRINGANIMATOR_

#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiSpringAnimator : public IGUIAnimator
{

public:
	CNrpGuiSpringAnimator( IGUIEnvironment* environment, IGUIElement* node, 
						   const core::recti& springRect, 
						   const core::recti& springRectEnd,
						   s32 time );

	~CNrpGuiSpringAnimator(void);

	virtual void draw();

protected:
	core::recti _springRectStart, _springRectEnd, _startRect; 
	core::rectf _currentRect;
	s32 _time;
};

}//namspace gui

}//namespace irr

#endif