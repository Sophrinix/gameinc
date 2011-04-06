#include "StdAfx.h"
#include "LuaRelation.h"

namespace nrp
{
CLASS_NAME CLASS_RELATION( "CLuaRelation" );

BEGIN_LUNA_METHODS(CLuaRelation)

END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaRelation)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaRelation )
	LUNA_AUTONAME_PROPERTY( CLuaRelation, "value", Get, Set )
END_LUNA_PROPERTIES

CLuaRelation::CLuaRelation(lua_State *L, bool ex) : ILuaObject(L, CLASS_RELATION, ex)
{}

int CLuaRelation::Set( lua_State *L )
{
	assert( lua_isnumber( L, -1 ) || lua_isstring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN 
	{
		if( lua_isnumber( L, -1 ) )
			(*_object)[ REL_VALUE ] = static_cast< float >( lua_tonumber( L, -1 ) );
		else if( lua_isstring( L, -1 ) )
			(*_object)[ REL_VALUE ] = NrpText( lua_tostring( L, -1 ) );
		else
			assert( "unsupport type" );
	}

	return 1;
}

int CLuaRelation::Get( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		if( (*_object)[ REL_VALUE ].Is<NrpText>() )
			lua_pushstring( L, (NrpText)(*_object)[ REL_VALUE ] );
		else if( (*_object)[ REL_VALUE ].Is< float >() )
			lua_pushnumber( L, (float)(*_object)[ REL_VALUE ] );
		else if( (*_object)[ REL_VALUE ].Is< int >() )
			lua_pushinteger( L, (int)(*_object)[ REL_VALUE ] );
		else
			lua_pushnil( L );
	}

	return 1;
}

const char* CLuaRelation::ClassName()
{
	return ( CLASS_RELATION );
}
}//namespace nrp