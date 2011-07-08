#include "StdAfx.h"
#include "NrpGuiHoveredAnimator.h"
#include "nrpGUIEnvironment.h"

namespace irr
{

namespace gui
{

CNrpGuiHoveredAnimator::~CNrpGuiHoveredAnimator(void)
{
}

CNrpGuiHoveredAnimator::CNrpGuiHoveredAnimator( IGUIEnvironment* environment, 
												IGUIElement* node, 
												u32 min_blend, 
												u32 max_blend, 
												u32 time,
												bool visOnStop, 
												bool remSelf,
												bool remParent )
: IGUIAnimator( environment, node ), 
  min_( min_blend ), max_( max_blend ), _time( time ),
  visOnStop_( visOnStop ), remSelf_( remSelf ), remParent_( remParent )
{
    
}

void CNrpGuiHoveredAnimator::draw()
{
	bool up = Environment->isHovered( Parent );

	//если не выделен родитель, проверим может выделен кто из детишек
	if( !up )													
	{
		core::list<IGUIElement*>::ConstIterator it = Parent->getChildren().begin();
		for (; it != Parent->getChildren().end(); ++it)
			if (Environment->isHovered( *it ) )
			{
				up = true;										//нашли такого
				break;											//выходим
			}
	}

    int step = ceil( abs( (int)(max_ - min_) ) * _time / (float)Environment->getVideoDriver()->getFPS() );

	bool workStop = false;
	if( up )
	{
		if( _currentAlpha < max_ )
			_currentAlpha+=core::s32_min( step, max_ - _currentAlpha );
		else
			workStop = true;
	}
	else
	{
		if( _currentAlpha > min_ )
			_currentAlpha -=core::s32_min( step, _currentAlpha - min_ );
		else
			workStop = true;
	}

	if( workStop )
	{
		Parent->setVisible( visOnStop_ );

		if( remSelf_ )
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( this );

		if( remParent_ )
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( Parent );

	}

	Parent->setAlphaBlend( _currentAlpha );
}

}//namespace gui

}//namespace irr