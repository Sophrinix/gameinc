#include "StdAfx.h"
#include "NrpGuiMoveAnimator.h"
#include "nrpGUIEnvironment.h"
#include <IGUIEnvironment.h>
#include <assert.h>

namespace irr
{

namespace gui
{

CNrpGuiMoveAnimator::CNrpGuiMoveAnimator( IGUIEnvironment* environment, 
										  IGUIElement* node, 
										  const core::position2di& stopPos,
										  s32 time,
										  bool visibleOnStop,
										  bool removeOnStop, 
										  bool removeParentOnStop )
	: IGUIAnimator( environment, node ), 
					_stopPos( stopPos ), _time( time ), visibleOnStop_( visibleOnStop ),
					remOnStop_( removeOnStop ), remParentOnStop_( removeParentOnStop )
{
	assert( node );
	_startPos = node ? node->getRelativePosition().UpperLeftCorner : core::position2di( 0, 0 );
}

void CNrpGuiMoveAnimator::draw()
{
	if( !IsEnabled )
		return;

	core::recti cuRect = Parent->getRelativePosition();

	if( cuRect.UpperLeftCorner != _stopPos )
	{
		s32 step = _stopPos.getDistanceFrom( _startPos ) * _time / Environment->getVideoDriver()->getFPS();
		step = (std::max)( 1, step );
		int offsetX = _stopPos.X - cuRect.UpperLeftCorner.X;
		int signX = offsetX < 0 ? -1 : 1;
		int offsetY = _stopPos.Y - cuRect.UpperLeftCorner.Y;
		int signY = offsetY < 0 ? -1 : 1;
		Parent->setRelativePosition( cuRect + core::position2di( signX * (std::min)( step, abs( offsetX ) ),
																 signY * (std::min)( step, abs( offsetY ) ) ) );
	}
	else
	{
		IsEnabled = false;
		Parent->setVisible( visibleOnStop_ );

		if( remOnStop_ )
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( this );

		if( remParentOnStop_ )
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( Parent );
	}
}

}//namespace gui

}//namespace irr
