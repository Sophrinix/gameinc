#include "StdAfx.h"
#include "LuaPlatform.h"
#include "NrpPlatform.h"
#include "NrpTechnology.h"
#include "LuaTechnology.h"

namespace nrp
{
CLASS_NAME CLASS_LUAPLATFORM( "CLuaPlatform" );

BEGIN_LUNA_METHODS(CLuaPlatform)
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, Load )
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, Create )
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, IsMyTech )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaPlatform)
	LUNA_ILUAPROJECT_PROPERTIES(CLuaPlatform)
	LUNA_AUTONAME_PROPERTY( CLuaPlatform, "texture", GetTexture, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlatform, "cpu", GetCpu, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlatform, "ram", GetRam, PureFunction )
END_LUNA_PROPERTIES

CLuaPlatform::CLuaPlatform(lua_State* L, bool ex) : ILuaProject<CNrpPlatform>(L, CLASS_LUAPLATFORM, ex )
{
	
}

int CLuaPlatform::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlatform:CreateTechnology need platformName parameter" );

	NrpText typeName = lua_tostring( L, 2 );

	_object = new CNrpPlatform( typeName );

	//lua_pop( L, argc );
	lua_pushlightuserdata(L, _object );
	Luna< CLuaPlatform >::constructor( L );

	return 1;
}

const char* CLuaPlatform::ClassName()
{
	return CLASS_LUAPLATFORM;
}

int CLuaPlatform::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlatform:CreateTechnology need platformName parameter" );

	NrpText pathTo = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->Load( pathTo );
	return 1;
}

int CLuaPlatform::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, TEXTURENORMAL, "") );
	return 1;
}

int CLuaPlatform::GetCpu( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, PROP, CPU, 0.f) );
	return 1;
}

int CLuaPlatform::GetRam( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, PROP, RAM, 0.f) );
	return 1;
}

int CLuaPlatform::IsMyTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:RemovePlatform need CLuaPlatform parameter" );

	CNrpTechnology* obj = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2, false );
	assert( obj );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN 
		if( obj ) ret = ( _object->GetTech( (*obj)[ INTERNAL_NAME ] ) != NULL );

	lua_pushboolean( L, ret );
	return 1; 
}

}//end namespace nrp