#pragma once
#include "NrpGuiBlendAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiLuaAnimator : public IGUIAnimator
{
public:
	CNrpGuiLuaAnimator( IGUIEnvironment* environment, IGUIElement* node, int funcRef );
	~CNrpGuiLuaAnimator(void);

	virtual void draw();
private:
	int _funcRef;
};

}//namspace gui

}//namespace irr
