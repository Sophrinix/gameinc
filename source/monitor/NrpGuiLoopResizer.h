#pragma once
#include "NrpGuiRectAnimator.h"

namespace irr
{

namespace gui
{

class NrpGuiLoopResizer : public CNrpGuiRectAnimator
{
public:
	NrpGuiLoopResizer( IGUIEnvironment* environment, 
						IGUIElement* node, 
						const core::recti& startRect, 
						const core::recti& stopRect,
						s32 time,
						bool visibleOnStop,
						bool removeOnStop, 
						bool removeParentOnStop );
	~NrpGuiLoopResizer(void);

	void draw();

private:
	s32 _startTime, _time;
	bool rOnStop_, rParentOnStop_;
	bool _mayDraw;
};

}//end namespace gui

}//end namespace irr