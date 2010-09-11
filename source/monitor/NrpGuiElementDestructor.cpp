#include "StdAfx.h"
#include "NrpGuiElementDestructor.h"
#include "nrpGUIEnvironment.h"

#include <assert.h>

namespace irr
{

namespace gui
{

CNrpGuiElementDestructor::~CNrpGuiElementDestructor(void)
{
}

CNrpGuiElementDestructor::CNrpGuiElementDestructor( IGUIEnvironment* environment, IGUIElement* node, int time )
		: IGUIElement( EGUIET_ELEMENT, environment, node, -1, core::rect<s32>( 0, 0, 0, 0 ) )
{
	_delay = time;
	_startTime = GetTickCount();
}

void CNrpGuiElementDestructor::draw()
{
	if( GetTickCount() - _startTime > _delay )
	{
		if( CNrpGUIEnvironment* env = dynamic_cast< CNrpGUIEnvironment* >( Environment ) )
			env->addToDeletionQueue( Parent );
		else
			assert( "usupported function" );
	}
}

}//namespace gui

}//namespace irr