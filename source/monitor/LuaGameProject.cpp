#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaGameProject.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpScenario.h"
#include "NrpLicense.h"
#include "NrpTechnology.h"
#include "NrpApplication.h"
#include "LuaTechnology.h"
#include "LuaGameEngine.h"
										
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
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetEngineExtend ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetLocalization ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCrossPlatformCode ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, Remove ),
	{0,0}
};

CLuaGameProject::CLuaGameProject(lua_State *L)	: ILuaProject(L, "CLuaGameProject")							//конструктор
{}

int CLuaGameProject::SetGameEngine( lua_State* L )\
{  
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:SetGameEngine need CNrpGameEngine parameter" );

	CNrpGameEngine* eng = (CNrpGameEngine*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN object_->SetGameEngine( eng );
	return 1;	
}
 
int CLuaGameProject::GetPlatformsNumber( lua_State* L )
{ lua_pushinteger( L, GetParam_<int>( L, "GetPlatformsNumber", PLATFORMNUMBER, 0 )); return 1; }

int CLuaGameProject::GetLanguagesNumber( lua_State* L )
{ lua_pushinteger( L, GetParam_<int>( L, "GetLanguagesNumber", LANGNUMBER, 0 )); return 1; }

int CLuaGameProject::GetScenario( lua_State* L )
{ return _TechLuaInitialize( L, "GetScenario", SCENARIO ); }

int CLuaGameProject::SetScenario( lua_State* L )
{ return SetNamedTech_( L, "SetScenario", SCENARIO ); }	

int CLuaGameProject::GetLicense( lua_State* L )
{ 
	PNrpLicense lic = GetParam_<PNrpLicense>( L, "GetLicense", GLICENSE, NULL );
	lua_pop( L, lua_gettop( L ) );
	lua_pushlightuserdata( L, lic ); 
	Luna< CLuaTechnology >::constructor( L );

	return 1; 
}

int CLuaGameProject::GetGenreModuleNumber( lua_State* L )
{ lua_pushinteger( L, GetParam_<int>( L, "GetGenreModuleNumber", GENRE_MODULE_NUMBER, 0 )); return 1; }

int CLuaGameProject::GetCodeQuality( lua_State* L )
{ lua_pushinteger( L, GetParam_<int>( L, "GetCodeQuality", QUALITY, 0 )); return 1; }

int CLuaGameProject::GetCodeVolume( lua_State* L )
{ lua_pushinteger( L, GetParam_<int>( L, "GetCodeQuality", CODEVOLUME, 0 )); return 1; }

int CLuaGameProject::GetGameEngine( lua_State* L )
{ 
	PNrpGameEngine ge = GetParam_<PNrpGameEngine>( L, "GetGameEngine", GAME_ENGINE, NULL );
	lua_pop( L, lua_gettop( L ) ); 
	lua_pushlightuserdata( L, ge ); 
	Luna< CLuaGameEngine >::constructor( L );

	return 1; 
}

int CLuaGameProject::IsProjectReady( lua_State* L )
{ lua_pushboolean( L, GetParam_<bool>( L, "IsProjectReady", PROJECTREADY, false )); return 1; }

int CLuaGameProject::GetScriptEngine( lua_State* L )
{ return _TechLuaInitialize( L, "GetScriptEngine", SCRIPTENGINE ); }

int CLuaGameProject::SetScriptEngine( lua_State* L )
{ return SetNamedTech_( L, "SetScriptEngine", SCRIPTENGINE ); }	

int CLuaGameProject::GetEngineExtend( lua_State* L )
{ lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetEngineExtend", ENGINEEXTENDED, NULL )); return 1; }

int CLuaGameProject::GetLocalization( lua_State* L )
{ lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetLocalization", LOCALIZATION, NULL )); return 1; }

int CLuaGameProject::GetCrossPlatformCode( lua_State* L )
{ lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetCrossPlatformCode", CROSSPLATFORMCODE, NULL )); return 1; }

int CLuaGameProject::GetMiniGameEngine( lua_State* L )
{ return _TechLuaInitialize( L, "GetMiniGameEngine", MINIGAMEENGINE ); }

int CLuaGameProject::SetMiniGameEngine( lua_State* L )
{ return SetNamedTech_( L, "SetMiniGameEngine", MINIGAMEENGINE ); }

int CLuaGameProject::GetPhysicEngine( lua_State* L )
{ return _TechLuaInitialize( L, "GetPhysicEngine", PHYSICSENGINE ); }

int CLuaGameProject::SetPhysicEngine( lua_State* L )
{ return SetNamedTech_( L, "SetPhysicEngine", PHYSICSENGINE ); }

int CLuaGameProject::GetAdvTechNumber( lua_State* L )
{ lua_pushinteger( L, GetParam_<int>( L, "GetAdvTechNumber", ADVTECHNUMBER, NULL )); return 1; }

int CLuaGameProject::GetVideoQuality( lua_State* L ) { return _TechLuaInitialize( L, "GetVideoQuality", GRAPHICQUALITY ); }

int CLuaGameProject::SetVideoQuality( lua_State* L )
{ return SetNamedTech_( L, "SetVideoQuality", GRAPHICQUALITY ); }	

int CLuaGameProject::GetVideoTechNumber( lua_State* L )
{ lua_pushinteger( L, GetParam_<int>( L, "GetVideoTechNumber", VIDEOTECHNUMBER, NULL )); return 1; }

int CLuaGameProject::GetSoundQuality( lua_State* L ) { return _TechLuaInitialize( L, "GetVideoQuality", SOUNDQUALITY ); }

int CLuaGameProject::SetSoundQuality( lua_State* L )
{ return SetNamedTech_( L, "SetSoundQuality", SOUNDQUALITY ); }	

int CLuaGameProject::GetSoundTechNumber( lua_State* L )
{ lua_pushinteger( L, GetParam_<int>( L, "GetSoundTechNumber", SOUNDTECHNUMBER, NULL )); return 1; }

int CLuaGameProject::_TechLuaInitialize( lua_State* L, std::string funcName, std::string paramName )
{
	PNrpTechnology tech = GetParam_<PNrpTechnology>( L, funcName, paramName, NULL );
	lua_pop( L, lua_gettop( L ) );
	lua_pushlightuserdata( L, tech); 
	Luna< CLuaTechnology >::constructor( L );
	return 1;
}

int CLuaGameProject::GetGenre( lua_State* L )
{
	return GetNumericalTech_( L, "", &CNrpGameProject::GetGenre );
}	

int CLuaGameProject::SetGenre( lua_State* L ) 
{ 
	return SetNumericalTech_( L, "SetGenre", &CNrpGameProject::SetGenre ); 
}

int CLuaGameProject::GetAdvTech( lua_State* L )
{
	return GetNumericalTech_( L, "", &CNrpGameProject::GetTechnology );
}

int CLuaGameProject::SetAdvTech( lua_State* L ) 
{
	return SetNumericalTech_( L, "SetAdvTech", &CNrpGameProject::SetTechnology ); 
}

int CLuaGameProject::GetVideoTech( lua_State* L )
{
	return GetNumericalTech_( L, "", &CNrpGameProject::GetVideoTech );
}

int CLuaGameProject::SetVideoTech( lua_State* L ) 
{
	return SetNumericalTech_( L, "SetVideoTech", &CNrpGameProject::SetVideoTech ); 
}

int CLuaGameProject::GetSoundTech( lua_State* L )
{
	return GetNumericalTech_( L, "", &CNrpGameProject::GetSoundTech );
}

int CLuaGameProject::SetSoundTech( lua_State* L ) 
{
	return SetNumericalTech_( L, "SetSoundTech", &CNrpGameProject::SetSoundTech ); 
}

int CLuaGameProject::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:Create need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );

	object_ = new CNrpGameProject( name, NULL );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, object_ );
	Luna< CLuaGameProject >::constructor( L );

	return 1;
}

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

int CLuaGameProject::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGameProject:Remove not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN 
	{
		delete object_;
		object_ = NULL;
	}

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

	lua_pop( L, argc );
	lua_pushlightuserdata( L, tech );
	Luna< CLuaTechnology >::constructor( L );

	return 1;	
}

}//namespace nrp