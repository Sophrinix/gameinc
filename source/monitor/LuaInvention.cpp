#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaInvention.h"
#include "NrpInvention.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "NrpCompany.h"

namespace nrp
{

Luna< CLuaInvention >::RegType CLuaInvention::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaInvention ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaInvention, AddUser ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Load ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Remove ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetStatus ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetDescriptionLink ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetInvestiment ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, SetInvestiment ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetSpeed ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetPrognoseDateFinish ),
	LUNA_AUTONAME_FUNCTION( CLuaInvention, CheckParams ),
	{0,0}
};

CLuaInvention::CLuaInvention(lua_State *L) : ILuaProject( L, "CLuaInvention" )							//конструктор
{}

int CLuaInvention::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaInvention:Create need int, company parameter" );

	CNrpTechnology* tech = (CNrpTechnology*)lua_touserdata( L, 2 );
	PNrpCompany cmp = (PNrpCompany)lua_touserdata( L, 3 );

	object_ = new CNrpInvention( tech, cmp );
	lua_pushlightuserdata(L, object_ );

	return 1;
}

int CLuaInvention::AddUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention::HaveLider not need parameter");

	bool haveUser = false; 
	IF_OBJECT_NOT_NULL_THEN	haveUser = !object_->GetValue<std::string>( COMPONENTLIDER ).empty();
	lua_pushboolean( L, haveUser );
	return 1;	
}

int CLuaInvention::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete object_;
		object_ = NULL;
	}

	return 1;	
}

int CLuaInvention::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaInvention::Load not need parameter");

	const char* iniFile = lua_tostring( L, 2 );
	assert( iniFile != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->Load( iniFile );
	return 1;	
}

int CLuaInvention::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetTexture", TEXTURENORMAL, "" ).c_str() );
	return 1;
}

int CLuaInvention::GetStatus( lua_State* L )
{
	lua_pushinteger( L, GetParam_<TECH_STATUS>( L, "GetStatus", STATUS, TS_UNKNOWN) );
	return 1;
}

int CLuaInvention::GetDescriptionLink( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetDescriptionLink", DESCRIPTIONPATH, "" ).c_str() );
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
	IF_OBJECT_NOT_NULL_THEN	time = object_->GetValue<SYSTEMTIME>( PROGNOSEDATEFINISH );

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

	IF_OBJECT_NOT_NULL_THEN	object_->CheckParams();

	return 1;
}
}//namespace nrp