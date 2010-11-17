#include "StdAfx.h"
#include "NrpTimer.h"
#include "nrpGUIEnvironment.h"
#include "nrpScript.h"

namespace irr
{

namespace gui
{

CNrpTimer::~CNrpTimer(void)
{
}

CNrpTimer::CNrpTimer( IGUIEnvironment* environment, IGUIElement* node, size_t time, const NrpText& action )
	: IGUIAnimator( environment, node )
{
	_time = time > 0 ? time : 0;
	_startTime = GetTickCount();
	_active = true;
	_action = action;
}

void CNrpTimer::draw()
{
	if( _active && (GetTickCount() - _startTime > _time) )
	{
		nrp::CNrpScript::Instance().SetSender( this );
		nrp::CNrpScript::Instance().DoString( _action );
		_active = false;  

		if( CNrpGUIEnvironment* env = dynamic_cast< CNrpGUIEnvironment* >( Environment) )
			env->addToDeletionQueue( this );
	}
}

}//namespace gui

}//namespace irr