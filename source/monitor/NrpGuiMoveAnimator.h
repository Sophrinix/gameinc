#pragma once
#include "NrpGuiBlendAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiMoveAnimator : public IGUIAnimator
{

public:
	CNrpGuiMoveAnimator( IGUIEnvironment* environment, 
						 IGUIElement* node, 
						 const core::position2di& stopPos,
						 s32 step=1,
						 bool visibleOnStop=true,
						 bool removeOnStop=true, 
						 bool removeParentOnStop=false );

	~CNrpGuiMoveAnimator(void) {};

	virtual void draw();
private:
	core::position2di stopPos_;
	s32 step_;
	bool visibleOnStop_, remOnStop_, remParentOnStop_;
};

}//namspace gui

}//namespace irr
