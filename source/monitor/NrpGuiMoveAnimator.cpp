#include "StdAfx.h"
#include "NrpGuiMoveAnimator.h"
#include "nrpGUIEnvironment.h"
#include <IGUIEnvironment.h>

namespace irr
{

namespace gui
{

CNrpGuiMoveAnimator::CNrpGuiMoveAnimator( IGUIEnvironment* environment, 
										  IGUIElement* node, 
										  const core::position2di& stopPos,
										  s32 step,
										  bool visibleOnStop,
										  bool removeOnStop, 
										  bool removeParentOnStop )
	: IGUIAnimator( environment, node ), 
					stopPos_( stopPos ), step_( step ), visibleOnStop_( visibleOnStop ),
					remOnStop_( removeOnStop ), remParentOnStop_( removeParentOnStop )
{
}

void CNrpGuiMoveAnimator::draw()
{
	if( !IsEnabled )
		return;

	core::recti cuRect = Parent->getRelativePosition();

	if( cuRect.UpperLeftCorner != stopPos_ )
	{
		int offsetX = stopPos_.X - cuRect.UpperLeftCorner.X;
		int signX = offsetX < 0 ? -1 : 1;
		int offsetY = stopPos_.Y - cuRect.UpperLeftCorner.Y;
		int signY = offsetY < 0 ? -1 : 1;
		Parent->setRelativePosition( cuRect + core::position2di( signX * min( step_, abs( offsetX ) ),
																 signY * min( step_, abs( offsetY ) ) ) );
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
