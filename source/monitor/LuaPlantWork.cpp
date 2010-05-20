#include "StdAfx.h"
#include "LuaPlantWork.h"
#include "NrpPlantWork.h"

using namespace irr;

namespace nrp
{

Luna< CLuaPlantWork >::RegType CLuaPlantWork::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaPlantWork ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, Create ),
	{0,0}
};

int CLuaPlantWork::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:Create not need any parameter" );

	object_ = new CNrpPlantWork();
	lua_pushlightuserdata( L, object_ );

	return 1;	
}

CLuaPlantWork::CLuaPlantWork( lua_State *L ) : ILuaObject( L, CLASS_LUAPLANTWORK )
{

}
}