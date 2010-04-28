#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaGameProject.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpScenario.h"
#include "NrpLicense.h"
#include "NrpTechnology.h"

#define BEGIN_AUTONAME_FUNCTION(name) int CLuaGameProject::name( lua_State* L )\
									  {  std::string functionAutoName = #name;
#define END_AUTONAME_FUNCTION }

#define GETTER_FUNCTION( name, lua_pushfunc, typen, paramName, defValue )\
	int CLuaGameProject::name( lua_State* L ) { lua_pushfunc( L, GetParam_<typen>( L, #name, paramName, defValue )); return 1; }

#define SETTER_FUNCTION( name, paramName )\
	int CLuaGameProject::name( lua_State* L ){ return SetNamedTech_( L, #name, paramName );	}

#define GETTER_NUMERICAL_FUNCTION( name, func )\
	int CLuaGameProject::name( lua_State* L ) { return GetNumericalTech_( L, #name, &CNrpGameProject::func ); }				

#define SETTER_NUMERICAL_FUNCTION( name, func )\
	int CLuaGameProject::name( lua_State* L ) { return SetNumericalTech_( L, #name, &CNrpGameProject::func ); }
												
namespace nrp
{

Luna< CLuaGameProject >::RegType CLuaGameProject::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaGameProject ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetGameEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetGenre ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetGenreModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetGenre ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCodeVolume ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsGenreIncluded ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsMyGameEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetGameEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetLicense ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetScenario ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetPlatformsNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetLanguagesNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsProjectReady ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, HaveLicense ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, HaveScenario ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetScriptEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetScriptEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetMiniGameEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetMiniGameEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetPhysicEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetPhysicEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetAdvTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetAdvTech ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetAdvTech ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsTechInclude ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetVideoQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetVideoQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetVideoTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetVideoTech ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetVideoTech ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetSoundQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetSoundQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetSoundTech ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetSoundTech ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetSoundTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCodeQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsPlatformAvaible ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsLangAvaible ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, TogglePlatform ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, ToggleLanguage ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetScenario ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetName ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetName ),
	{0,0}
};

CLuaGameProject::CLuaGameProject(lua_State *L)	: ILuaProject(L, "CLuaGameProject")							//конструктор
{}

BEGIN_AUTONAME_FUNCTION(SetGameEngine)
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, ("Function CLuaGameProject:" + functionAutoName + "need CNrpGameEngine parameter").c_str() );

	CNrpGameEngine* eng = (CNrpGameEngine*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN object_->SetGameEngine( eng );
	return 1;	
END_AUTONAME_FUNCTION
 
GETTER_FUNCTION(GetPlatformsNumber, lua_pushinteger,		int,			PLATFORMNUMBER,		0 )
GETTER_FUNCTION(GetLanguagesNumber, lua_pushinteger,		int,			LANGNUMBER,			0 )
GETTER_FUNCTION(GetScenario,		lua_pushlightuserdata,	PNrpScenario,	SCENARIO,			NULL )
SETTER_FUNCTION(SetScenario,		SCENARIO )
GETTER_FUNCTION(GetLicense,			lua_pushlightuserdata,	PNrpLicense,	GLICENSE,			NULL )
GETTER_FUNCTION(GetGenreModuleNumber, lua_pushinteger,		int,			GENRE_MODULE_NUMBER,0 )
GETTER_FUNCTION(GetCodeQuality,     lua_pushinteger,		int,			QUALITY,			0 )
GETTER_FUNCTION(GetCodeVolume,		lua_pushinteger,		int,			CODEVOLUME,			0 )
GETTER_FUNCTION(GetGameEngine,		lua_pushlightuserdata,	PNrpGameEngine, GAME_ENGINE,		NULL )
GETTER_FUNCTION(IsProjectReady,		lua_pushboolean,		bool,			PROJECTREADY,		false )

GETTER_FUNCTION(GetScriptEngine,	lua_pushlightuserdata,	PNrpTechnology, SCRIPTENGINE,		NULL )
SETTER_FUNCTION(SetScriptEngine,	SCRIPTENGINE )

GETTER_FUNCTION(GetMiniGameEngine,	lua_pushlightuserdata,	PNrpTechnology, MINIGAMEENGINE,		NULL )
SETTER_FUNCTION(SetMiniGameEngine,  MINIGAMEENGINE )

GETTER_FUNCTION(GetPhysicEngine,	lua_pushlightuserdata,	PNrpTechnology, PHYSICSENGINE,		NULL ) 
SETTER_FUNCTION(SetPhysicEngine,	PHYSICSENGINE )

GETTER_FUNCTION(GetAdvTechNumber,	lua_pushinteger,		int,			ADVTECHNUMBER,		0 ) 
GETTER_FUNCTION(GetVideoQuality, 	lua_pushlightuserdata,  PNrpTechnology, GRAPHICQUALITY,		NULL )

SETTER_FUNCTION(SetVideoQuality,	GRAPHICQUALITY )

GETTER_FUNCTION(GetVideoTechNumber, lua_pushinteger,		int,			VIDEOTECHNUMBER,	0 ) 

GETTER_FUNCTION(GetSoundQuality,    lua_pushlightuserdata,  PNrpTechnology, SOUNDQUALITY,		NULL )
SETTER_FUNCTION(SetSoundQuality,    SOUNDQUALITY )

GETTER_FUNCTION(GetSoundTechNumber, lua_pushinteger,        int,			SOUNDTECHNUMBER,	0 )

GETTER_NUMERICAL_FUNCTION( GetGenre, GetGenre )
SETTER_NUMERICAL_FUNCTION( SetGenre, SetGenre )

GETTER_NUMERICAL_FUNCTION( GetAdvTech, GetTechnology )
SETTER_NUMERICAL_FUNCTION( SetAdvTech, SetTechnology )

GETTER_NUMERICAL_FUNCTION( GetVideoTech, GetVideoTech )
SETTER_NUMERICAL_FUNCTION( SetVideoTech, SetVideoTech )

GETTER_NUMERICAL_FUNCTION( GetSoundTech, GetSoundTech )
SETTER_NUMERICAL_FUNCTION( SetSoundTech, SetSoundTech )

int CLuaGameProject::IsGenreIncluded( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsGenreIncluded need GENRE_TYPE parameter" );

	GENRE_TYPE techType = GENRE_TYPE( lua_tointeger( L, 2 ) );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN isIncl = object_->IsGenreIncluded( techType );
	lua_pushboolean( L, isIncl );

	return 1;			
}

int CLuaGameProject::IsMyGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsMyGameEngine need CNrpGameEngine* parameter" );

	CNrpGameEngine* ge = (CNrpGameEngine*)lua_touserdata( L, 2 );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN isIncl = (object_->GetValue<PNrpGameEngine>( GAME_ENGINE ) == ge);
	lua_pushboolean( L, isIncl );

	return 1;		
}

int CLuaGameProject::HaveLicense( lua_State* L )
{
	lua_pushboolean( L, GetParam_<PNrpLicense>( L, "HaveLicense", GLICENSE, NULL) != NULL );
	return 1;
}

int CLuaGameProject::HaveScenario( lua_State* L )
{
	lua_pushboolean( L, GetParam_<PNrpScenario>( L, "HaveScenario", SCENARIO, NULL ) != NULL );
	return 1;
}

int CLuaGameProject::SetNamedTech_( lua_State* L, std::string funcName, const std::string paramName )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, ("Function CLuaGameProject:" + funcName + " need PNrpTechnology parameter").c_str() );
	PNrpTechnology tech = (PNrpTechnology)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		object_->SetValue<PNrpTechnology>( paramName, tech );
		object_->CalculateCodeVolume();
	}

	return 1;		
}

int CLuaGameProject::IsTechInclude( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsTechInclude need ADV_TECH_TYPE parameter" );

	ADV_TECH_TYPE techType = ADV_TECH_TYPE( lua_tointeger( L, 2 ) );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN isIncl = object_->IsTechInclude( techType );
	lua_pushboolean( L, isIncl );

	return 1;			
}

template< class T >
int CLuaGameProject::SetNumericalTech_( lua_State* L, 
										std::string funcName, 
										void (T::*Method)( CNrpTechnology* tehc, int index) )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, ("Function CLuaGameProject:" + funcName + " need CNrpTechnology parameter").c_str() );

	PNrpTechnology ptrTech = (PNrpTechnology)lua_touserdata( L, 2 );
	int number = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN
	{
		(object_->*Method)( ptrTech, number );
		object_->CalculateCodeVolume();
	}
	return 1;	
}

int CLuaGameProject::IsLangAvaible( lua_State* L )
{
	return IsParamAvaible_( L, "IsLangAvaible", "language_" );
}

int CLuaGameProject::ToggleLanguage( lua_State* L )
{
	return ToggleParam_( L, "ToggleLanguage", "language_" );
}

int CLuaGameProject::IsPlatformAvaible( lua_State* L )
{
	return IsParamAvaible_( L, "IsPlatformAvaible", "platform_" );
}

int CLuaGameProject::TogglePlatform( lua_State* L )
{
	return ToggleParam_( L, "TogglePlatform", "platform_" );
}

int CLuaGameProject::ToggleParam_( lua_State* L, std::string funcName, std::string prefix )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, ("Function CLuaGameProject:" + funcName + " need string parameter").c_str() );

	const char* str = lua_tostring( L, 2 );
	assert( str != NULL );

	IF_OBJECT_NOT_NULL_THEN	
	{
		object_->ToggleValue<bool>( prefix + str, true );
		object_->CalculateCodeVolume();
	}
	return 1;	
}

int CLuaGameProject::IsParamAvaible_( lua_State* L, std::string funcName, std::string prefix )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, ("Function CLuaGameProject:" + funcName + " need string parameter").c_str() );

	const char* str = lua_tostring( L, 2 );
	assert( str != NULL );
	bool isAvaible = false;

	IF_OBJECT_NOT_NULL_THEN
	{
		try	{ isAvaible = object_->GetValue<bool>( prefix + str );	}
		catch(...) { }
	}
	lua_pushboolean( L, isAvaible );

	return 1;	
}

template< class T >
int nrp::CLuaGameProject::GetNumericalTech_( lua_State* L, 
											 std::string funcName, 
											 CNrpTechnology* (T::*Method)( int index ) )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, ("Function CLuaGameProject:" + funcName + " need CNrpTechnology, int parameter").c_str() );

	int idx = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;

	IF_OBJECT_NOT_NULL_THEN tech = (object_->*Method)( idx );
	lua_pushlightuserdata( L, tech );

	return 1;	
}

int CLuaGameProject::SetName( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, ("Function " + ClassName() + ":SetName need string parameter").c_str() );

	const char* name = lua_tostring( vm, 2 );
	assert( name != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->SetValue<std::string>( NAME, name );

	return 1;
}

int CLuaGameProject::GetName( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, ("Function " + ClassName() + ":GetName not need parameter").c_str() );

	std::string resultt("");
	IF_OBJECT_NOT_NULL_THEN	resultt = object_->GetValue<std::string>( NAME );

	lua_pushstring( vm, resultt.c_str() );
	return 1;
}


}//namespace nrp