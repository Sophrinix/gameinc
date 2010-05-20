#include "StdAfx.h"
#include "LuaDiskMachine.h"
#include "NrpDiskMachine.h"

using namespace irr;

namespace nrp
{

Luna< CLuaDiskMachine >::RegType CLuaDiskMachine::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaDiskMachine ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, Create ),
	{0,0}
};

int CLuaDiskMachine::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDiskMachine:Create not need any parameter" );

	object_ = new CNrpDiskMachine();
	lua_pushlightuserdata( L, object_ );

	return 1;	
}

CLuaDiskMachine::CLuaDiskMachine( lua_State *L ) : ILuaObject( L, CLASS_LUADISKMACHINE )
{

}
}