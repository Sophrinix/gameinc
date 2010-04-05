#include "StdAfx.h"
#include "NrpGuiRectAnimator.h"
#include "nrpGUIEnvironment.h"

namespace irr
{

namespace gui
{

CNrpGuiRectAnimator::~CNrpGuiRectAnimator(void)
{
}

CNrpGuiRectAnimator::CNrpGuiRectAnimator( IGUIEnvironment* environment, 
										  IGUIElement* node, 
										  const core::recti& startRect, 
										  const core::recti& stopRect,
										  s32 step,
										  bool visibleOnStop,
										  bool removeOnStop, 
										  bool removeParentOnStop )
	: IGUIAnimator( environment, node ), 
			startRect_( startRect ), stopRect_( stopRect ), step_( step ),
			visibleOnStop_( visibleOnStop ), remOnStop_( removeOnStop ), 
			remParentOnStop_( removeParentOnStop )
{
	node->setRelativePosition( startRect );
}

void CNrpGuiRectAnimator::draw()
{
	if( !IsEnabled )
		return;

	core::recti cuRect = Parent->getRelativePosition();
	if( cuRect.UpperLeftCorner != stopRect_.UpperLeftCorner )
	{
		int offsetX = stopRect_.UpperLeftCorner.X - cuRect.UpperLeftCorner.X;
		int signX = offsetX < 0 ? -1 : 1;
		int offsetY = stopRect_.UpperLeftCorner.Y - cuRect.UpperLeftCorner.Y;
		int signY = offsetY < 0 ? -1 : 1;

		cuRect.UpperLeftCorner += core::position2di( signX * min( step_, abs( offsetX ) ), signY * min( step_, abs( offsetY ) ) );
	}
	else 
	if( cuRect.LowerRightCorner != stopRect_.LowerRightCorner )
	{
		int offsetX = stopRect_.LowerRightCorner.X - cuRect.LowerRightCorner.X;
		int signX = offsetX < 0 ? -1 : 1;
		int offsetY = stopRect_.LowerRightCorner.Y - cuRect.LowerRightCorner.Y;
		int signY = offsetY < 0 ? -1 : 1;

		cuRect.LowerRightCorner += core::position2di( signX * min( step_, abs( offsetX ) ), signY * min( step_, abs( offsetY ) ) );
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

	
	Parent->setRelativePosition( cuRect );
}

}//namespace gui

}//namespace irr
