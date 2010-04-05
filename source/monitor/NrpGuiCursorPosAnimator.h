#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiCursorAnimator : public IGUIAnimator
{
public:
	CNrpGuiCursorAnimator( IGUIEnvironment* environment, IGUIElement* node, core::position2di offset );
	~CNrpGuiCursorAnimator(void);

	virtual void draw();
private:
	core::position2di offset_;
};

}//namspace gui

}//namespace irr
