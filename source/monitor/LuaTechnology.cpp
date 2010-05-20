#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaTechnology.h"
#include "NrpTechnology.h"
#include "IUser.h"
#include "NrpApplication.h"

#define NO_POSTFIX
#define NO_ASSERT
#define GETTER_FUNCTION( name, lua_pushfunc, typen, paramName, defValue, postFix )\
	int CLuaTechnology::name( lua_State* L ) { lua_pushfunc( L, GetParam_<typen>( L, #name, paramName, defValue )postFix ); return 1; }

#define SETTER_FUNCTION( name, READTYPE, lua_tofunc, assertcode, OPTIONTYPE, paramName )\
	int CLuaTechnology::name( lua_State* L )\
	{	int argc = lua_gettop( L );\
		std::string funcName = #name;\
		luaL_argcheck( L, argc == 2, 2, ("Function CLuaTechnology:" + funcName + "need int parameter").c_str() );\
		READTYPE valuel = (READTYPE)lua_tofunc( L, 2 );\
		assertcode;\
		IF_OBJECT_NOT_NULL_THEN	object_->SetValue<OPTIONTYPE>( paramName, OPTIONTYPE(valuel) );\
		return 1;\
	}

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
	{0,0}
};

CLuaTechnology::CLuaTechnology(lua_State *L) : ILuaProject( L, "CLuaTech" )							//конструктор
{}

GETTER_FUNCTION( GetTechGroup, lua_pushinteger, int, TECHGROUP, 0, NO_POSTFIX  )

SETTER_FUNCTION( SetBaseCode, float, lua_tonumber, NO_ASSERT, float, BASE_CODE )
SETTER_FUNCTION( SetTechType, int, lua_tointeger, NO_ASSERT, int, TECHTYPE )
SETTER_FUNCTION( SetAddingEngineCode, float, lua_tonumber, NO_ASSERT, float, ENGINE_CODE )
SETTER_FUNCTION( SetQuality, int, lua_tonumber, NO_ASSERT, int, QUALITY )

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

	object_ = CNrpApplication::Instance().CreateTechnology( typen );
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
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::GetEmployerPosibility not need parameter");

	float posilbleValue = 0; 
	IF_OBJECT_NOT_NULL_THEN	posilbleValue = object_->GetEmployerPosibility();
	lua_pushnumber( L, posilbleValue );
	return 1;	
}

int CLuaTechnology::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::GetEmployerPosibility not need parameter");

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
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::GetLevel not need parameter");

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetValue<int>( LEVEL );

	lua_pushinteger( L, result );
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

}//namespace nrp