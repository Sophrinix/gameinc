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
	nrp::CNrpScript::Instance().ReleaseRef( _action );
}

CNrpTimer::CNrpTimer( IGUIEnvironment* environment, IGUIElement* node, size_t time, int action )
	: IGUIAnimator( environment, node ), _loop( false )
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
		nrp::CNrpScript::Instance().CallFunction( _action, (void*)this );
		_active = false;  

		if( _loop )
			_startTime = GetTickCount();
		else
			if( CNrpGUIEnvironment* env = dynamic_cast< CNrpGUIEnvironment* >( Environment) )
				env->addToDeletionQueue( this );
	}
}

}//namespace gui

}//namespace irr