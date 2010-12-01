#include "StdAfx.h"
#include "LuaRelation.h"

namespace nrp
{
CLASS_NAME CLASS_RELATION( "CLuaRelation" );

Luna< CLuaRelation >::RegType CLuaRelation::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaRelation ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaRelation, Set ),
	LUNA_AUTONAME_FUNCTION( CLuaRelation, Get ),
	{0,0}
};

CLuaRelation::CLuaRelation(lua_State *L) : ILuaObject(L, "CLuaRelation")
{}

int CLuaRelation::Set( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaRelation::Set need name, value parameter");

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		if( lua_isnumber( L, 3 ) )
			(*object_)[ name ] = static_cast< float >( lua_tonumber( L, 3 ) );
		else if( lua_isstring( L, 3 ) )
			(*object_)[ name ] = NrpText( lua_tostring( L, 3 ) );
		else
			assert( "unsupport type" );
	}

	return 1;
}

int CLuaRelation::Get( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaRelation::GetValue need name parameter");

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( (*object_)[ name ].Is<NrpText>() )
			lua_pushstring( L, (NrpText)(*object_)[ name ] );
		else if( (*object_)[ name ].Is< float >() )
			lua_pushnumber( L, (float)(*object_)[ name ] );
		else if( (*object_)[ name ].Is< int >() )
			lua_pushinteger( L, (int)(*object_)[ name ] );
		else
			lua_pushinteger( L, -1 );
	}

	return 1;
}

const char* CLuaRelation::ClassName()
{
	return ( CLASS_RELATION );
}
}//namespace nrp