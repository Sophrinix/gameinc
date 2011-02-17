#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaInvention.h"
#include "NrpInvention.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "NrpCompany.h"
#include "LuaUser.h"
#include "LuaCompany.h"

namespace nrp
{
CLASS_NAME CLASS_LUAINVENTION( "CLuaInvention" );

BEGIN_LUNA_METHODS(CLuaInvention)
	LUNA_AUTONAME_FUNCTION( CLuaInvention, AddUser )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Load )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Remove )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetLevel )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Create )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetStatus )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetDescriptionLink )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetInvestiment )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, SetInvestiment )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetSpeed )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetPrognoseDateFinish )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, CheckParams )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetUserNumber )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetUser )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetPassedMoney )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetDayLeft )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, IsValid )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetCompany )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetInternalName )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetMonthPay )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, ClearMonthPay )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaInvention)
	LUNA_ILUAPROJECT_PROPERTIES( CLuaInvention )
END_LUNA_PROPERTIES

CLuaInvention::CLuaInvention(lua_State *L, bool ex) : ILuaProject( L, CLASS_LUAINVENTION, ex )							//конструктор
{}

int CLuaInvention::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaInvention:Create need int, company parameter" );

	CNrpTechnology* tech = (CNrpTechnology*)lua_touserdata( L, 2 );
	PNrpCompany cmp = (PNrpCompany)lua_touserdata( L, 3 );

	_object = new CNrpInvention( tech, cmp );
	lua_pushlightuserdata(L, _object );

	return 1;
}

int CLuaInvention::AddUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaInvention::AddUser not need parameter");

	IUser* user = _GetLuaObject< IUser, CLuaUser >( L, 2, false ); 
	IF_OBJECT_NOT_NULL_THEN	_object->AddUser( user );

	return 1;	
}

int CLuaInvention::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete _object;
		_object = NULL;
	}

	return 1;	
}

int CLuaInvention::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaInvention::Load not need parameter");

	NrpText iniFile = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	_object->Load( iniFile );
	return 1;	
}

int CLuaInvention::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetTexture", TEXTURENORMAL, "" ) );
	return 1;
}

int CLuaInvention::GetStatus( lua_State* L )
{
	lua_pushinteger( L, GetParam_<TECH_STATUS>( L, "GetStatus", STATUS, TS_UNKNOWN) );
	return 1;
}

int CLuaInvention::GetDescriptionLink( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetDescriptionLink", DESCRIPTIONPATH, "" ) );
	return 1;
}

int CLuaInvention::GetLevel( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetLevel", LEVEL, 0 ) );
	return 1;
}

int CLuaInvention::GetInvestiment( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetInvestiment", INVESTIMENT, 0 ) );
	return 1;
}

int CLuaInvention::GetSpeed( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetInvestiment", INVENTIONSPEED, 0 ) );
	return 1;
}

int CLuaInvention::GetPrognoseDateFinish( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:Remove not need parameter");

	SYSTEMTIME time;
	memset( &time, 0, sizeof(SYSTEMTIME) );
	IF_OBJECT_NOT_NULL_THEN	time = (*_object)[ PROGNOSEDATEFINISH ];

	lua_pushinteger( L, time.wDay );
	lua_pushinteger( L, time.wMonth );
	lua_pushinteger( L, time.wYear );
	return 3;
}

int CLuaInvention::SetInvestiment( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, "SetInvestiment", INVESTIMENT, lua_tointeger );
}

int CLuaInvention::CheckParams( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:CheckParams not need parameter");

	IF_OBJECT_NOT_NULL_THEN	_object->CheckParams();

	return 1;
}

int CLuaInvention::GetUserNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetUserNumber", USERNUMBER, 0 ) );
	return 1;
}

int CLuaInvention::GetUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaInvention::Load not GetUser parameter");

	int index = lua_tointeger( L, 2 );
	IUser* user = NULL;

	IF_OBJECT_NOT_NULL_THEN	user = _object->GetUser( index );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, user );
	Luna< CLuaUser >::constructor( L );

	return 1;	
}

int CLuaInvention::GetPassedMoney( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetPassedMoney", PASSEDPRICE, 0 ) );
	return 1;
}

int CLuaInvention::GetDayLeft( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetDayLeft", DAYLEFT, 0 ) );
	return 1;
}

int CLuaInvention::IsValid( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:IsValid not need parameter");

	bool avaible = true;
	try
	{
		IF_OBJECT_NOT_NULL_THEN	_object->ClassName();
	}
	catch(...)
	{
		avaible = false;
	}

	lua_pushboolean( L, avaible );
	return 1;
}

int CLuaInvention::GetCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:GetCompany not need parameter");

	PNrpCompany cmp = NULL;
	IF_OBJECT_NOT_NULL_THEN	cmp = (*_object)[ PARENTCOMPANY ].As<PNrpCompany>();

	lua_pop( L, argc );
	lua_pushlightuserdata( L, cmp );
	Luna< CLuaCompany >::constructor( L );

	return 1;
}

int CLuaInvention::GetInternalName( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetInternalName", INTERNAL_NAME, "" ) );
	return 1;
}

int CLuaInvention::GetMonthPay( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetMonthPay", MONEY_TODECREASE, 0 ) );
	return 1;
}

int CLuaInvention::ClearMonthPay( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:ClearMonthPay not need parameter");

	IF_OBJECT_NOT_NULL_THEN (*_object)[ MONEY_TODECREASE ] = (int)0;

	return 1;
}

const char* CLuaInvention::ClassName()
{
	return ( CLASS_LUAINVENTION );
}
}//namespace nrp