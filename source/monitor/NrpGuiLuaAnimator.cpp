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

CNrpGuiLuaAnimator::CNrpGuiLuaAnimator( IGUIEnvironment* environment, IGUIElement* node, const NrpText& funcName )
	: IGUIAnimator( environment, node )
{
	funcName_ = funcName;
}

void CNrpGuiLuaAnimator::draw()
{
	if( funcName_.size() )
		nrp::CNrpScript::Instance().CallFunction( funcName_ );
}

}//namespace gui

}//namespace irr