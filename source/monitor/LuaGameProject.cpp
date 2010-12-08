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
#include "NrpPlatform.h"
#include "LuaPlatform.h"
										
namespace nrp
{
CLASS_NAME CLASS_LUAGMEPROJECT( "CLuaGameProject" );

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
	LUNA_AUTONAME_FUNCTION(	CLuaGameProject, GetPlatform ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsMyPlatform ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, AddPlatform ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, RemovePlatform ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetLanguagesNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetLanguage ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsMyLanguage ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, AddLanguage ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, RemoveLanguage ),
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
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetScenario ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetEngineExtend ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetLocalization ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCrossPlatformCode ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCpuUse ),
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetMemoryUse ),

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
	IF_OBJECT_NOT_NULL_THEN _object->SetGameEngine( eng );
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
	PNrpTechnology lic = GetParam_<PNrpTechnology>( L, "GetLicense", GLICENSE, NULL );
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
{ 
	lua_pushinteger( L, GetParam_<int>( L, "GetSoundTechNumber", SOUNDTECHNUMBER, 0 )); 
	return 1; 
}

int CLuaGameProject::_TechLuaInitialize( lua_State* L, const NrpText& funcName, OPTION_NAME& paramName )
{
	PNrpTechnology tech = GetParam_<PNrpTechnology>( L, funcName, paramName, NULL );

	lua_pop( L, lua_gettop( L ) );
	lua_pushlightuserdata( L, tech); 
	Luna< CLuaTechnology >::constructor( L );
	return 1;
}

int CLuaGameProject::GetGenre( lua_State* L )
{
	return GetNumericalParam_< CLuaTechnology >( L, "", &CNrpGameProject::GetGenre );
}	

int CLuaGameProject::SetGenre( lua_State* L ) 
{ 
	return SetNumericalTech_( L, "SetGenre", &CNrpGameProject::SetGenre ); 
}

int CLuaGameProject::GetAdvTech( lua_State* L )
{
	return GetNumericalParam_< CLuaTechnology >( L, "", &CNrpGameProject::GetTechnology );
}

int CLuaGameProject::SetAdvTech( lua_State* L ) 
{
	return SetNumericalTech_( L, "SetAdvTech", &CNrpGameProject::SetTechnology ); 
}

int CLuaGameProject::GetVideoTech( lua_State* L )
{
	return GetNumericalParam_< CLuaTechnology >( L, "", &CNrpGameProject::GetVideoTech );
}

int CLuaGameProject::SetVideoTech( lua_State* L ) 
{
	return SetNumericalTech_( L, "SetVideoTech", &CNrpGameProject::SetVideoTech ); 
}

int CLuaGameProject::GetSoundTech( lua_State* L )
{
	return GetNumericalParam_< CLuaTechnology >( L, "", &CNrpGameProject::GetSoundTech );
}

int CLuaGameProject::SetSoundTech( lua_State* L ) 
{
	return SetNumericalTech_( L, "SetSoundTech", &CNrpGameProject::SetSoundTech ); 
}

int CLuaGameProject::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:Create need string parameter" );

	NrpText name = lua_tostring( L, 2 );

	_object = new CNrpGameProject( name, NULL );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, _object );
	Luna< CLuaGameProject >::constructor( L );

	return 1;
}

int CLuaGameProject::IsGenreIncluded( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsGenreIncluded need GenreName parameter" );

	CNrpTechnology* genre = _GetObjectFromTable< CNrpTechnology, CLuaTechnology >( L, 2, -1 );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN isIncl = _object->IsGenreIncluded( genre );
	lua_pushboolean( L, isIncl );

	return 1;			
}

int CLuaGameProject::IsMyGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsMyGameEngine need CNrpGameEngine* parameter" );

	CNrpGameEngine* ge = (CNrpGameEngine*)lua_touserdata( L, 2 );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN isIncl = (*_object)[ GAME_ENGINE ] == ge;
	lua_pushboolean( L, isIncl );

	return 1;		
}

int CLuaGameProject::HaveLicense( lua_State* L )
{
	lua_pushboolean( L, GetParam_<PNrpTechnology>( L, "HaveLicense", GLICENSE, NULL) != NULL );
	return 1;
}

int CLuaGameProject::HaveScenario( lua_State* L )
{
	lua_pushboolean( L, GetParam_<PNrpTechnology>( L, "HaveScenario", SCENARIO, NULL ) != NULL );
	return 1;
}

int CLuaGameProject::SetNamedTech_( lua_State* L, const NrpText& funcName, const NrpText& paramName )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + " need PNrpTechnology parameter") );
	PNrpTechnology tech = (PNrpTechnology)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		(*_object)[ paramName ] = tech;
		_object->CalculateCodeVolume();
	}

	return 1;		
}

int CLuaGameProject::IsTechInclude( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsTechInclude need ADV_TECH_TYPE parameter" );

	CNrpTechnology* tech = _GetObjectFromTable< CNrpTechnology, CLuaTechnology >( L, 2, -1 );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN isIncl = _object->IsTechInclude( tech );
	lua_pushboolean( L, isIncl );

	return 1;			
}

template< class Param, class T >
int CLuaGameProject::SetNumericalTech_( lua_State* L, 
										const NrpText& funcName, 
										void (T::*Method)( Param* tehc, int index) )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, _ErrStr( NrpText(":") + funcName + " need CNrpTechnology parameter") );

	Param* ptrParam = NULL;
	if( lua_isuserdata( L, 2 ) )
		ptrParam = (Param*)lua_touserdata( L, 2 );
	else if( lua_istable( L, 2) )
		ptrParam = (Param*)(_GetLuaObject( L, 2)->GetSelf());

	int number = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN
	{
		(_object->*Method)( ptrParam, number );
		_object->CalculateCodeVolume();
	}
	return 1;	
}

int CLuaGameProject::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGameProject:Remove not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN 
	{
		delete _object;
		_object = NULL;
	}

	return 1;	
}

const char* CLuaGameProject::ClassName()
{
	return ( CLASS_LUAGMEPROJECT );
}

int CLuaGameProject::GetPlatform( lua_State* L )
{
	return GetNumericalParam_< CLuaPlatform >( L, "", &CNrpGameProject::GetPlatform );
}

int CLuaGameProject::IsMyPlatform( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsMyPlatform need CNrpPlatform* parameter" );
	CNrpPlatform* pl = _GetObjectFromTable< CNrpPlatform, CLuaPlatform >( L, 2, -1 );
	bool isIncl = false;
	IF_OBJECT_NOT_NULL_THEN isIncl = _object->GetPlatform( pl ? (NrpText)(*pl)[ INTERNAL_NAME ] : "" ) != NULL;
	lua_pushboolean( L, isIncl );
	return 1;			
}

template< class B, class A, class T >
int nrp::CLuaGameProject::GetNumericalParam_( lua_State* L, 
											 const NrpText& funcName, 
											 A* (T::*Method)( int index ) )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + " need int parameter") );

	int idx = lua_tointeger( L, 2 );
	A* tech = NULL;

	IF_OBJECT_NOT_NULL_THEN tech = (_object->*Method)( idx );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, tech );
	Luna< B >::constructor( L );

	return 1;	
}

int nrp::CLuaGameProject::GetLanguage( lua_State* L )
{
	return GetNumericalParam_< CLuaTechnology >( L, "", &CNrpGameProject::GetLanguage );
}

int CLuaGameProject::IsMyLanguage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsMyLanguage need CNrpTechnology* parameter" );

	CNrpTechnology* lang = _GetObjectFromTable< CNrpTechnology, CLuaTechnology >( L, 2, -1 );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN isIncl = _object->GetLanguage( lang ? (NrpText)(*lang)[ INTERNAL_NAME ] : "" ) != NULL;
	lua_pushboolean( L, isIncl );

	return 1;			
}

int CLuaGameProject::AddLanguage( lua_State* L )
{
	return SetNumericalTech_( L, "AddLanguage", &CNrpGameProject::SetLanguage ); 
}

int CLuaGameProject::RemoveLanguage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:RemoveLanguage need CLuaTech parameter" );

	CNrpTechnology* obj = _GetObjectFromTable< CNrpTechnology, CLuaTechnology >( L, 2, -1 );
	IF_OBJECT_NOT_NULL_THEN _object->RemoveLanguage( obj );
	
	return 1;
}

int CLuaGameProject::AddPlatform( lua_State* L )
{
	return SetNumericalTech_( L, "AddPlatform", &CNrpGameProject::SetPlatform ); 
}

int CLuaGameProject::RemovePlatform( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:RemovePlatform need CLuaPlatform parameter" );

	CNrpPlatform* obj = _GetObjectFromTable< CNrpPlatform, CLuaPlatform >( L, 2, -1 );
	IF_OBJECT_NOT_NULL_THEN _object->RemovePlatform( obj );

	return 1; 
}

int CLuaGameProject::GetCpuUse( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, "GetCpuUse", CPU, 0.f ) ); 
	return 1; 
}

int CLuaGameProject::GetMemoryUse( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, "GetMemoryUse", RAM, 0.f ) ); 
	return 1; 
}

}//namespace nrp