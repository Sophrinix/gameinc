#include "StdAfx.h"
#include "NrpGuiCursorPosAnimator.h"
#include "nrpEngine.h"
#include <irrlicht.h>

namespace irr
{

namespace gui
{

CNrpGuiCursorAnimator::~CNrpGuiCursorAnimator(void)
{
}

CNrpGuiCursorAnimator::CNrpGuiCursorAnimator( IGUIEnvironment* environment, IGUIElement* node, core::position2di offset )
	: IGUIAnimator( environment, node ), offset_( offset )
{
}

void CNrpGuiCursorAnimator::draw()
{

	core::position2di pos = CNrpEngine::Instance().GetDevice()->getCursorControl()->getPosition();

	core::position2di parentPos = Parent->getParent() ? Parent->getParent()->getAbsolutePosition().UpperLeftCorner : core::position2di( 0, 0 );

	Parent->setRelativePosition( pos - parentPos + offset_ );
}

}//namespace gui

}//namespace irr