#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaDevelopProject.h"
#include "INrpDevelopProject.h"
#include "LuaDevelopModule.h"
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_LUADEVPROJECT( "CLuaDevelopProject" );

BEGIN_LUNA_METHODS(CLuaDevelopProject)
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetModuleNumber )
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetModule )
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetFamous )
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetWorkPercentDone )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaDevelopProject)
	LUNA_ILUAPROJECT_PROPERTIES( CLuaDevelopProject )
END_LUNA_PROPERTIES 

CLuaDevelopProject::CLuaDevelopProject(lua_State *L, bool ex)	: ILuaProject(L, CLASS_LUADEVPROJECT, ex)							//конструктор
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
	IF_OBJECT_NOT_NULL_THEN prj = _object->GetModule( index );

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

int CLuaDevelopProject::GetWorkPercentDone( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, "GetWorkPercentDone", READYWORKPERCENT, 0 ) );
	return 1;
}
}//namespace nrp