#include "StdAfx.h"
#include "NrpGuiSpringAnimator.h"
#include "nrpGUIEnvironment.h"
#include <assert.h>

namespace irr
{

namespace gui
{

CNrpGuiSpringAnimator::~CNrpGuiSpringAnimator(void)
{
}

CNrpGuiSpringAnimator::CNrpGuiSpringAnimator( IGUIEnvironment* environment, 
										  IGUIElement* node, 
										  const core::recti& springRectStart, 
										  const core::recti& springRectEnd,
										  s32 time )
					  : IGUIAnimator( environment, node ), 
						_springRectStart( springRectStart ),
						_springRectEnd( springRectEnd ),
						_time( time )
{
	assert( node );
	_currentRect = core::rectf( _springRectStart.UpperLeftCorner.X, _springRectStart.UpperLeftCorner.Y,
								_springRectStart.LowerRightCorner.X, _springRectStart.LowerRightCorner.Y );
}

void CNrpGuiSpringAnimator::draw()
{
	if( !IsEnabled )
		return;

	if( core::abs_( _currentRect.UpperLeftCorner.X - _springRectEnd.UpperLeftCorner.X ) > 2 ||
		core::abs_( _currentRect.UpperLeftCorner.Y - _springRectEnd.UpperLeftCorner.Y ) > 2 )
	{
		core::position2di tmp = _springRectEnd.UpperLeftCorner - _springRectStart.UpperLeftCorner;
		core::position2df step( tmp.X, tmp.Y );
		step *= ( _time / (float)Environment->getVideoDriver()->getFPS() ); 

		_currentRect.UpperLeftCorner += step;
	}
	else
	{
		core::position2di tmp = _springRectEnd.UpperLeftCorner;
		_springRectEnd.UpperLeftCorner = _springRectStart.UpperLeftCorner;
		_springRectStart.UpperLeftCorner = tmp;
	}

	if( core::abs_(_currentRect.LowerRightCorner.X - _springRectEnd.LowerRightCorner.X) > 2 ||
		core::abs_(_currentRect.LowerRightCorner.Y - _springRectEnd.LowerRightCorner.Y) > 2 )
	{
		core::position2di tmp = _springRectEnd.LowerRightCorner - _springRectStart.LowerRightCorner;
		core::position2df step( tmp.X, tmp.Y );
		step *= ( _time / (float)Environment->getVideoDriver()->getFPS() ); 

		_currentRect.LowerRightCorner += step;
	}
	else
	{
		core::position2di tmp = _springRectEnd.LowerRightCorner;
		_springRectEnd.LowerRightCorner = _springRectStart.LowerRightCorner;
		_springRectStart.LowerRightCorner = tmp;
	}
	
	Parent->setRelativePosition( core::recti( _currentRect.UpperLeftCorner.X, _currentRect.UpperLeftCorner.Y,
											  _currentRect.LowerRightCorner.X, _currentRect.LowerRightCorner.Y ) );
}

}//namespace gui

}//namespace irr
