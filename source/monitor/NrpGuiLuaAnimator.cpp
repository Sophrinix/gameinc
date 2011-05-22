#include "StdAfx.h"
#include "NrpGuiLuaAnimator.h"
#include "nrpScript.h"

namespace irr
{

namespace gui
{

CNrpGuiLuaAnimator::~CNrpGuiLuaAnimator(void)
{
}

CNrpGuiLuaAnimator::CNrpGuiLuaAnimator( IGUIEnvironment* environment, IGUIElement* node, int funcRef )
	: IGUIAnimator( environment, node )
{
	assert( funcRef > 0 );
	_funcRef = funcRef;
}

void CNrpGuiLuaAnimator::draw()
{
	if( _funcRef > 0 )
		nrp::CNrpScript::Instance().CallFunction( _funcRef );
}

}//namespace gui

}//namespace irr