#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaDevelopProject.h"
#include "INrpDevelopProject.h"
#include "LuaDevelopModule.h"
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_LUADEVPROJECT( "CLuaDevelopProject" );

Luna< CLuaDevelopProject >::RegType CLuaDevelopProject::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaDevelopProject ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetModule ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetFamous ),
	{0,0}
};

CLuaDevelopProject::CLuaDevelopProject(lua_State *L)	: ILuaProject(L, CLASS_LUADEVPROJECT)							//конструктор
{}

int CLuaDevelopProject::GetModuleNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetModuleNumber", MODULE_NUMBER, 0) );
	return 1;
}

int CLuaDevelopProject::GetModule( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDevelopProject:GetModule() need int parameter" );

	CNrpProjectModule* prj = NULL;
	int index = lua_tointeger( L, 2 );
	IF_OBJECT_NOT_NULL_THEN prj = object_->GetModule( index );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, prj );
	Luna< CLuaDevelopModule >::constructor( L );
	return 1;	
}

int CLuaDevelopProject::GetFamous( lua_State* L )
{
	lua_pushinteger( L, static_cast< int >( GetParam_<float>( L, "GetFamous", FAMOUS, 0 ) * 100 ) );
	return 1;
}

const char* CLuaDevelopProject::ClassName()
{
	return ( CLASS_LUADEVPROJECT );
}
}//namespace nrp