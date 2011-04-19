#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaReklame.h"
#include "NrpReklameWork.h"
#include "NrpPlant.h"

namespace nrp
{
CLASS_NAME CLASS_LUAREKLAME( "CLuaReklame" );

BEGIN_LUNA_METHODS(CLuaReklame)
	LUNA_AUTONAME_FUNCTION( CLuaReklame, Remove )
	LUNA_AUTONAME_FUNCTION( CLuaReklame, Create )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaReklame)
	LUNA_ILUAPROJECT_PROPERTIES( CLuaReklame )
	LUNA_AUTONAME_PROPERTY( CLuaReklame, "level", GetLevel, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaReklame, "numberDay", GetNumberDay, SetNumberDay )
	LUNA_AUTONAME_PROPERTY( CLuaReklame, "price", GetPrice, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaReklame, "dayCost", GetDayCost, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaReklame, "famous", GetFamous, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaReklame, "company", GetCompanyName, SetCompanyName )
	LUNA_AUTONAME_PROPERTY( CLuaReklame, "reklameObject", PureFunction, SetReklameObject )
	LUNA_AUTONAME_PROPERTY( CLuaReklame, "quality", GetQuality, PureFunction )
END_LUNA_PROPERTIES

CLuaReklame::CLuaReklame(lua_State *L, bool ex) : ILuaProject( L, CLASS_LUAREKLAME, ex )							//конструктор
{}

int CLuaReklame::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaReklame:CreateTechnology need string, string parameter" );

	NrpText typeName = lua_tostring( L, 2 );
	NrpText gameName = lua_tostring( L, 3 );
	NrpText company = lua_tostring( L, 4 );

	_object = CNrpPlant::Instance().CreateReklame( typeName, gameName, company );

	//lua_pop( L, argc );
	lua_pushlightuserdata(L, _object );
	Luna< CLuaReklame >::constructor( L );

	return 1;
}

int CLuaReklame::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaReklame::Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete _object;
		_object = NULL;
	}

	return 1;	
}

int CLuaReklame::SetReklameObject( lua_State* L ) 
{	
	IF_OBJECT_NOT_NULL_THEN
	{
		INrpConfig* reklameObject = _GetLuaObject< INrpConfig, ILuaProject >( L, 2, true );
		assert( reklameObject );
		if( reklameObject )
		{
			(*_object)[ GAMENAME ] = (*reklameObject)[ NAME ];
			(*_object)[ TYPEOBJECT ] = reklameObject->ObjectTypeName();
			(*_object)[ COMPANYNAME ] = (*reklameObject)[ COMPANYNAME ];
		}
	}

	return 0;
}

int CLuaReklame::GetQuality( lua_State* L ) { lua_pushinteger( L, GetParam_<int>( L, PROP, QUALITY, 0 ) ); return 1; }
int CLuaReklame::GetLevel( lua_State* L ) {	lua_pushinteger( L, GetParam_<int>( L, PROP, LEVEL, 0 ) ); return 1; }
int CLuaReklame::GetNumberDay( lua_State* L ) { lua_pushinteger( L, GetParam_<int>( L, PROP, NUMBERDAY, 0 ) ); return 1; }
int CLuaReklame::SetNumberDay( lua_State* L ) {	SetParam_<int, lua_Integer>( L, PROP, NUMBERDAY, lua_tointeger );	return 1; }
int CLuaReklame::GetPrice( lua_State* L ) {	lua_pushinteger( L, GetParam_<int>( L, PROP, BALANCE, 0 ) ); return 1; }
int CLuaReklame::GetDayCost( lua_State* L ) { lua_pushinteger( L, GetParam_<int>( L, PROP, DAYCOST, 0 ) );return 1; }
int CLuaReklame::GetFamous( lua_State* L ) { lua_pushinteger( L, static_cast< int >( GetParam_<float>( L, PROP, MAXQUALITY, 0 ) * 100 ) ); return 1; }
int CLuaReklame::SetCompanyName( lua_State* L ) {	SetParam_( L, PROP, COMPANYNAME ); return 1; }
int CLuaReklame::GetCompanyName( lua_State* L ) { lua_pushstring( L, GetParam_<NrpText>( L, PROP, COMPANYNAME, "" ) ); return 1; }

const char* CLuaReklame::ClassName()
{
	return ( CLASS_LUAREKLAME );
}
}//namespace nrp