#pragma once
#include "NrpGuiBlendAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiLuaAnimator : public IGUIAnimator
{
public:
	CNrpGuiLuaAnimator( IGUIEnvironment* environment, IGUIElement* node, const core::stringc& funcName );
	~CNrpGuiLuaAnimator(void);

	virtual void draw();
private:
	core::stringc funcName_;
};

}//namspace gui

}//namespace irr
