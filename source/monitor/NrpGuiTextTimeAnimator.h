#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiTextTimeAnimator : public IGUIAnimator
{
public:
	CNrpGuiTextTimeAnimator( IGUIEnvironment* environment, IGUIElement* node );
	~CNrpGuiTextTimeAnimator(void);

	virtual void draw();
private:
	size_t time_;
};

}//namspace gui

}//namespace irr
