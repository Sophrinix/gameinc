#pragma once
#include "NrpGuiBlendAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiLuaAnimator : public IGUIAnimator
{
public:
	CNrpGuiLuaAnimator( IGUIEnvironment* environment, IGUIElement* node, const nrp::NrpText& funcName );
	~CNrpGuiLuaAnimator(void);

	virtual void draw();
private:
	nrp::NrpText funcName_;
};

}//namspace gui

}//namespace irr
