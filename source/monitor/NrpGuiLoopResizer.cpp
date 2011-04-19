#include "StdAfx.h"
#include "NrpGuiLoopResizer.h"
#include "nrpGUIEnvironment.h"

namespace irr
{

namespace gui
{

NrpGuiLoopResizer::NrpGuiLoopResizer(IGUIEnvironment* environment, 
									 IGUIElement* node, 
									 const core::recti& startRect, 
									 const core::recti& stopRect,
								 	 s32 time,
									 bool visibleOnStop,
									 bool removeOnStop, 
									 bool removeParentOnStop )
 : CNrpGuiRectAnimator( environment, node, startRect, stopRect, 4, true, false, false )
{
	_time = time;
	rOnStop_ = removeOnStop;
	rParentOnStop_ = removeParentOnStop;
	_startTime = GetTickCount();
	_mayDraw = true;
}

NrpGuiLoopResizer::~NrpGuiLoopResizer(void)
{
}

void NrpGuiLoopResizer::draw()
{
	if( !_mayDraw  )
		return;

	CNrpGuiRectAnimator::draw();
	
	if( GetTickCount() - _startTime > _time )
	{
		if( rOnStop_ )
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( this );

		if( rParentOnStop_ )
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( Parent );

		_mayDraw = false;
		return;
	}

	if( !IsEnabled )
	{
		core::recti tmp = startRect_;
		startRect_ = stopRect_;
		startRect_ = tmp;
		IsEnabled = true;
	}
}



}//end namespace gui

}//end namespace irr