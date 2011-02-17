#include "StdAfx.h"
#include "LuaRelation.h"

namespace nrp
{
CLASS_NAME CLASS_RELATION( "CLuaRelation" );

BEGIN_LUNA_METHODS(CLuaRelation)
	LUNA_AUTONAME_FUNCTION( CLuaRelation, Set )
	LUNA_AUTONAME_FUNCTION( CLuaRelation, Get )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaRelation)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaRelation )
END_LUNA_PROPERTIES

CLuaRelation::CLuaRelation(lua_State *L, bool ex) : ILuaObject(L, CLASS_RELATION, ex)
{}

int CLuaRelation::Set( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaRelation::Set need name, value parameter");

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		if( lua_isnumber( L, 3 ) )
			(*_object)[ name ] = static_cast< float >( lua_tonumber( L, 3 ) );
		else if( lua_isstring( L, 3 ) )
			(*_object)[ name ] = NrpText( lua_tostring( L, 3 ) );
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
		if( (*_object)[ name ].Is<NrpText>() )
			lua_pushstring( L, (NrpText)(*_object)[ name ] );
		else if( (*_object)[ name ].Is< float >() )
			lua_pushnumber( L, (float)(*_object)[ name ] );
		else if( (*_object)[ name ].Is< int >() )
			lua_pushinteger( L, (int)(*_object)[ name ] );
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