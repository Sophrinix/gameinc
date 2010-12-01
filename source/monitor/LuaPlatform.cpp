#include "StdAfx.h"
#include "LuaPlatform.h"
#include "NrpPlatform.h"

namespace nrp
{
CLASS_NAME CLASS_LUAPLATFORM( "CLuaPlatform" );

Luna< CLuaPlatform >::RegType CLuaPlatform::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaPlatform ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, Load ),
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, GetTexture ),
	{0,0}
};

CLuaPlatform::CLuaPlatform(lua_State* L) : ILuaProject<CNrpPlatform>(L, CLASS_LUAPLATFORM)
{
	
}

int CLuaPlatform::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlatform:CreateTechnology need platformName parameter" );

	NrpText typeName = lua_tostring( L, 2 );

	object_ = new CNrpPlatform( typeName );

	lua_pop( L, argc );
	lua_pushlightuserdata(L, object_ );
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

	IF_OBJECT_NOT_NULL_THEN object_->Load( pathTo );
	return 1;
}

int CLuaPlatform::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetTexture", TEXTURENORMAL, "") );
	return 1;
}

}//end namespace nrp