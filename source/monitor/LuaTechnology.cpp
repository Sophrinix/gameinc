#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaTechnology.h"
#include "NrpTechnology.h"
#include "IUser.h"
#include "NrpApplication.h"

#define NO_POSTFIX
#define NO_ASSERT

namespace nrp
{

Luna< CLuaTechnology >::RegType CLuaTechnology::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaTechnology ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetTechType ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetBaseCode ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetAddingEngineCode ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEngineTechRequire ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEmployerSkillRequire ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetTechGroup ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetOptionAsInt ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, HaveLider ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetEmployerPosibility ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Load ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Remove ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, IsLoaded ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, ValidTime ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetTexture ),
	{0,0}
};

CLuaTechnology::CLuaTechnology(lua_State *L) : ILuaProject( L, "CLuaTech" )							//конструктор
{}

int CLuaTechnology::GetTechGroup( lua_State* L ) 
{ 
	lua_pushinteger( L, GetParam_<int>( L, "GetTechGroup", TECHGROUP, 0 ) ); 
	return 1; 
}

int CLuaTechnology::SetBaseCode( lua_State* L )
{
	return SetParam_<float, lua_Number>( L, "SetBaseCode", BASE_CODE, lua_tonumber );
}

int CLuaTechnology::SetTechType( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, "SetTechType", TECHTYPE, lua_tointeger );
}

int CLuaTechnology::SetAddingEngineCode( lua_State* L )
{
	return SetParam_<float, lua_Number>( L, "SetAddingEngineCode", ENGINE_CODE, lua_tonumber );
}

int CLuaTechnology::SetQuality( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, "SetQuality", QUALITY, lua_tointeger );
}

int CLuaTechnology::SetTexture( lua_State* L)
{
	return SetParam_( L, "SetTexture", TEXTURENORMAL );
}

int CLuaTechnology::GetOptionAsInt( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::GetOptionAsInt need int parameter");

	const char* opName = lua_tostring( L, 2 );
	assert( opName != NULL );

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetValue<int>( opName );
	lua_pushinteger( L, result );
	return 1;	
}

int CLuaTechnology::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:CreateTechnology need int parameter" );

	int typen = lua_tointeger( L, 2 );

	object_ = new CNrpTechnology( PROJECT_TYPE(typen) );
	lua_pushlightuserdata(L, object_ );

	return 1;
}

int CLuaTechnology::SetEngineTechRequire( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechnology::SetEngineTechRequire need int,int parameter");

	int typen = lua_tointeger( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	object_->SetEngineTechRequire( typen, valuel );

	return 1;	
}

int CLuaTechnology::SetEmployerSkillRequire( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechnology::SetEmployerSkillRequire need int,int parameter");

	int typen = lua_tointeger( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	object_->SetEmployerSkillRequire( typen, valuel );

	return 1;	
}

int CLuaTechnology::HaveLider( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::HaveLider not need parameter");

	bool haveUser = false; 
	IF_OBJECT_NOT_NULL_THEN	haveUser = !object_->GetValue<std::string>( COMPONENTLIDER ).empty();
	lua_pushboolean( L, haveUser );
	return 1;	
}

int CLuaTechnology::GetEmployerPosibility( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::GetEmployerPosibility not need parameter");

	float posilbleValue = 0; 
	PUser puser = (PUser)lua_touserdata( L, 2 );
	assert( puser != NULL );
	if( puser != NULL )
		IF_OBJECT_NOT_NULL_THEN	posilbleValue = object_->GetEmployerPosibility( puser );

	lua_pushnumber( L, posilbleValue );
	return 1;	
}

int CLuaTechnology::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete object_;
		object_ = NULL;
	}

	return 1;	
}

int CLuaTechnology::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::Load not need parameter");

	const char* iniFile = lua_tostring( L, 2 );
	assert( iniFile != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->Load( iniFile );
	return 1;	
}

int CLuaTechnology::GetLevel( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetLevel", LEVEL, 0 ) );
	return 1;
}

int CLuaTechnology::ValidTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::ValidTime not need any parameter");

	bool validTime = false; 
	IF_OBJECT_NOT_NULL_THEN
	{
		SYSTEMTIME starttime = object_->GetValue<SYSTEMTIME>( STARTDATE );
		SYSTEMTIME endtime = object_->GetValue<SYSTEMTIME>( ENDDATE );
		SYSTEMTIME currentTime = CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME );
		validTime = (starttime < currentTime) && (currentTime < endtime);
	}
	
	lua_pushboolean( L, validTime );
	return 1;	
}

int CLuaTechnology::IsLoaded( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::IsLoaded not need any parameter");

	bool loaded = false; 
	IF_OBJECT_NOT_NULL_THEN loaded = CNrpApplication::Instance().GetBoxAddon( object_->GetValue<std::string>( NAME ) ) != NULL;

	lua_pushboolean( L, loaded );
	return 1;			
}

int CLuaTechnology::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetTexture", TEXTURENORMAL, "" ).c_str() );
	return 1;
}
}//namespace nrp