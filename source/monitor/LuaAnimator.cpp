#include "StdAfx.h"
#include "LuaAnimator.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAANIMATOR( "CLuaAnimator" );

BEGIN_LUNA_METHODS(CLuaAnimator)
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaAnimator)
	LUNA_AUTONAME_PROPERTY( CLuaAnimator, "onRemove", PureFunction, SetOnRemove )
END_LUNA_PROPERTIES


const char* CLuaAnimator::ClassName()
{
	return 	( CLASS_LUAANIMATOR );
}

int CLuaAnimator::SetOnRemove( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN	_object->Bind( GUIELEMENT_ON_REMOVE, _GetRef( L, -1 ) );
	return 0;
}

CLuaAnimator::CLuaAnimator(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUAANIMATOR, ex )							//конструктор
{}

}