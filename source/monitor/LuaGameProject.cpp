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

BEGIN_LUNA_METHODS(CLuaGameProject)
	LUNA_ILUAPROJECT_HEADER( CLuaGameProject )
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetGameEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetNumber )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCodeVolume )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsMyTech )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsMyGameEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetGameEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetLicense )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetScenario )
	LUNA_AUTONAME_FUNCTION(	CLuaGameProject, GetPlatform )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsMyPlatform )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, AddPlatform )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, RemovePlatform )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsProjectReady )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, HaveLicense )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, HaveScenario )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetScriptEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetScriptEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetMiniGameEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetMiniGameEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetPhysicEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetPhysicEngine )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetTech )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, AddTech )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, RemoveTech )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsMyTech )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetVideoQuality )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetVideoQuality )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetSoundQuality )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetSoundQuality )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCodeQuality )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, SetScenario )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetEngineExtend )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetLocalization )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCrossPlatformCode )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetCpuUse )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, GetMemoryUse )

	LUNA_AUTONAME_FUNCTION( CLuaGameProject, Create )
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, Remove )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaGameProject)
END_LUNA_PROPERTIES

CLuaGameProject::CLuaGameProject(lua_State *L, bool ex)	: ILuaProject(L, CLASS_LUAGMEPROJECT, ex)							//конструктор
{}

int CLuaGameProject::SetGameEngine( lua_State* L )\
{  
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:SetGameEngine need CNrpGameEngine parameter" );

	CNrpGameEngine* eng = (CNrpGameEngine*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN _object->SetGameEngine( eng );
	return 1;	
}
 
int CLuaGameProject::GetNumber( lua_State* L )
{ 
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:GetNumber need PROJECT_TYPE parameter" );

	int type = lua_tointeger( L, 2 );

	switch( type )
	{
	case PT_PLATFORM: lua_pushinteger( L, (int)(*_object)[ PLATFORMNUMBER ] ); return 1; 
	case PT_LANGUAGE: lua_pushinteger( L, (int)(*_object)[ LANGNUMBER ] ); return 1; 
	case PT_GENRE:    lua_pushinteger( L, (int)(*_object)[ GENRE_MODULE_NUMBER ] ); return 1; 
	case PT_ADVTECH:    lua_pushinteger( L, (int)(*_object)[ ADVTECHNUMBER ] ); return 1; 
	case PT_VIDEOTECH: lua_pushinteger( L, (int)(*_object)[ VIDEOTECHNUMBER ] ); return 1; 
	case PT_SOUNDTECH: lua_pushinteger( L, (int)(*_object)[ SOUNDTECHNUMBER ] ); return 1; 
	default: return 1;
	}
}

int CLuaGameProject::GetScenario( lua_State* L )
{ return _XInitialize< CNrpScenario, CLuaTechnology >( L, "GetScenario", SCENARIO ); }

int CLuaGameProject::SetScenario( lua_State* L )
{ return _SetNamedTech< CNrpScenario >( L, "SetScenario", SCENARIO ); }	

int CLuaGameProject::GetLicense( lua_State* L )
{ 
	PNrpLicense lic = GetParam_<PNrpLicense>( L, "GetLicense", GLICENSE, NULL );
	lua_pop( L, lua_gettop( L ) );
	lua_pushlightuserdata( L, lic ); 
	Luna< CLuaTechnology >::constructor( L );

	return 1; 
}

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
{ return _XInitialize< CNrpTechnology, CLuaTechnology >( L, "GetScriptEngine", SCRIPTENGINE ); }

int CLuaGameProject::SetScriptEngine( lua_State* L )
{ return _SetNamedTech< CNrpGameEngine >( L, "SetScriptEngine", SCRIPTENGINE ); }	

int CLuaGameProject::GetEngineExtend( lua_State* L )
{ lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetEngineExtend", ENGINEEXTENDED, NULL )); return 1; }

int CLuaGameProject::GetLocalization( lua_State* L )
{ lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetLocalization", LOCALIZATION, NULL )); return 1; }

int CLuaGameProject::GetCrossPlatformCode( lua_State* L )
{ lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetCrossPlatformCode", CROSSPLATFORMCODE, NULL )); return 1; }

int CLuaGameProject::GetMiniGameEngine( lua_State* L )
{ return _XInitialize< CNrpTechnology, CLuaTechnology >( L, "GetMiniGameEngine", MINIGAMEENGINE ); }

int CLuaGameProject::SetMiniGameEngine( lua_State* L )
{ return _SetNamedTech< CNrpTechnology >( L, "SetMiniGameEngine", MINIGAMEENGINE ); }

int CLuaGameProject::GetPhysicEngine( lua_State* L )
{ return _XInitialize< CNrpTechnology, CLuaTechnology >( L, "GetPhysicEngine", PHYSICSENGINE ); }

int CLuaGameProject::SetPhysicEngine( lua_State* L )
{ return _SetNamedTech< CNrpTechnology >( L, "SetPhysicEngine", PHYSICSENGINE ); }

int CLuaGameProject::GetVideoQuality( lua_State* L ) 
{ return _XInitialize< CNrpTechnology, CLuaTechnology >( L, "GetVideoQuality", GRAPHICQUALITY ); }

int CLuaGameProject::SetVideoQuality( lua_State* L )
{ return _SetNamedTech< CNrpTechnology >( L, "SetVideoQuality", GRAPHICQUALITY ); }	

int CLuaGameProject::GetSoundQuality( lua_State* L ) 
{ return _XInitialize< CNrpTechnology, CLuaTechnology >( L, "GetVideoQuality", SOUNDQUALITY ); }

int CLuaGameProject::SetSoundQuality( lua_State* L )
{ return _SetNamedTech< CNrpTechnology >( L, "SetSoundQuality", SOUNDQUALITY ); }	

template< class T, class B >
int CLuaGameProject::_XInitialize( lua_State* L, const NrpText& funcName, OPTION_NAME& paramName )
{
	T* tech = GetParam_<T*>( L, funcName, paramName, NULL );

	lua_pop( L, lua_gettop( L ) );
	lua_pushlightuserdata( L, tech); 
	Luna< B >::constructor( L );
	return 1;
}

int CLuaGameProject::GetTech( lua_State* L )
{ return _GetAny< CLuaTechnology >( L, "GetTech", &CNrpGameProject::GetTech ); }

int CLuaGameProject::AddTech( lua_State* L ) 
{ return _XAny( L, "AddTech", &CNrpGameProject::AddTech ); }

int CLuaGameProject::RemoveTech( lua_State* L ) 
{ return _XAny( L, "RemoveTech", &CNrpGameProject::RemoveTech ); }

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

int CLuaGameProject::IsMyTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsGenreIncluded need GenreName parameter" );

	CNrpTechnology* tech = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2, false );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN 
	{
		isIncl = _object->IsMyTech( tech );
		isIncl |= ( (*_object)[ GLICENSE ] == (CNrpLicense*)tech );
		isIncl |= ( (*_object)[ SCENARIO ] == (CNrpScenario*)tech );
	}

	lua_pushboolean( L, isIncl );

	return 1;			
}

int CLuaGameProject::IsMyGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsMyGameEngine need CNrpGameEngine* parameter" );

	CNrpGameEngine* ge = _GetLuaObject< CNrpGameEngine, CLuaGameEngine >( L, 2 );
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

template< class T >
int CLuaGameProject::_SetNamedTech( lua_State* L, const NrpText& funcName, const NrpText& paramName )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + " need PNrpTechnology parameter") );
	T* tech = (T*)_GetLuaObject< T, ILuaObject >( L, 2, true );

	IF_OBJECT_NOT_NULL_THEN
	{
		(*_object)[ paramName ] = tech;
		_object->CalculateCodeVolume();
	}

	return 1;		
}

template< class Param, class T >
int CLuaGameProject::_XAny( lua_State* L, 
							   const NrpText& funcName, 
							   void (T::*Method)( const Param*) )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + " need CNrpTechnology parameter") );

	Param* ptrParam = _GetLuaObject< Param, ILuaObject >( L, 2, true);

	IF_OBJECT_NOT_NULL_THEN
	{
		(_object->*Method)( ptrParam );
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
	return _GetAny< CLuaPlatform >( L, "", &CNrpGameProject::GetPlatform );
}

int CLuaGameProject::IsMyPlatform( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsMyPlatform need CNrpPlatform* parameter" );
	CNrpPlatform* pl = _GetLuaObject< CNrpPlatform, CLuaPlatform >( L, 2, false );
	bool isIncl = false;
	IF_OBJECT_NOT_NULL_THEN isIncl = _object->GetPlatform( pl ? (NrpText)(*pl)[ INTERNAL_NAME ] : "" ) != NULL;
	lua_pushboolean( L, isIncl );
	return 1;			
}

template< class B, class A, class T >
int nrp::CLuaGameProject::_GetAny( lua_State* L, 
								   const NrpText& funcName, 
								   A* (T::*Method)( PROJECT_TYPE, int ) )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, _ErrStr( NrpText(":") + funcName + " need int, PROJECT_TYPE parameter") );

	int idx = lua_tointeger( L, 2 );
	PROJECT_TYPE type = static_cast< PROJECT_TYPE >( lua_tointeger( L, 3 ) );
	A* tech = NULL;

	IF_OBJECT_NOT_NULL_THEN tech = (_object->*Method)( type, idx );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, tech );
	Luna< B >::constructor( L );

	return 1;	
}

int CLuaGameProject::AddPlatform( lua_State* L )
{
	return _XAny( L, "AddPlatform", &CNrpGameProject::AddPlatform ); 
}

int CLuaGameProject::RemovePlatform( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:RemovePlatform need CLuaPlatform parameter" );

	CNrpPlatform* obj = _GetLuaObject< CNrpPlatform, CLuaPlatform >( L, 2, false );
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