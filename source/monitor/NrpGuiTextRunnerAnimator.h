#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

class CNrpGuiTextRunnerAnimator : public IGUIAnimator
{
public:
	CNrpGuiTextRunnerAnimator( IGUIEnvironment* environment, IGUIElement* node, 
							   const wchar_t* text );
	~CNrpGuiTextRunnerAnimator(void);

	virtual void draw();
	void setText( const wchar_t* txt );

private:
	size_t time_;
	core::stringw _text;
	size_t _currentPos;
};

}//namspace gui

}//namespace irr
