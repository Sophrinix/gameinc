#pragma once
#include "NrpGuiBlendAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiRectAnimator : public IGUIAnimator
{

public:
	CNrpGuiRectAnimator( IGUIEnvironment* environment, IGUIElement* node, 
						 const core::recti& startRect, 
						 const core::recti& stopRect,
						 s32 step,
						 bool visibleOnStop=true,
						 bool removeOnStop=true, 
						 bool removeParentOnStop=false );

	~CNrpGuiRectAnimator(void);

	virtual void draw();
private:
	core::recti startRect_, stopRect_;
	s32 step_;

	bool visibleOnStop_, remOnStop_, remParentOnStop_;
};

}//namspace gui

}//namespace irr
