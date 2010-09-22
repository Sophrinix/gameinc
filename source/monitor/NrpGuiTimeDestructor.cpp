#include "StdAfx.h"
#include "NrpGuiTimeDestructor.h"
#include "nrpGUIEnvironment.h"

namespace irr
{

namespace gui
{

CNrpGuiTimeDestructor::~CNrpGuiTimeDestructor(void)
{
}

CNrpGuiTimeDestructor::CNrpGuiTimeDestructor( IGUIEnvironment* environment, IGUIElement* node, int time )
	: IGUIAnimator( environment, node )
{
	_time = time;
	_startTime = GetTickCount();
	_active = true;
}

void CNrpGuiTimeDestructor::draw()
{
	if( _active && (GetTickCount() - _startTime > _time) )
		if( CNrpGUIEnvironment* env = dynamic_cast< CNrpGUIEnvironment* >( Environment) )
		{
			env->addToDeletionQueue( Parent );
			_active = false;  
		}
}

}//namespace gui

}//namespace irr