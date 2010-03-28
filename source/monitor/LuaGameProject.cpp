#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaGameProject.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpScenario.h"
#include "NrpLicense.h"
#include "NrpTechnology.h"

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
	LUNA_AUTONAME_FUNCTION( CLuaGameProject, IsAdvTechInclude ),
	{0,0}
};

CLuaGameProject::CLuaGameProject(lua_State *L)	: ILuaProject(L, "CLuaGameProject")							//конструктор
{}

int CLuaGameProject::SetGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:SetGameEngine need CNrpGameEngine parameter" );

	CNrpGameEngine* eng = (CNrpGameEngine*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN object_->SetGameEngine( eng );
	return 1;	
}

int CLuaGameProject::GetPlatformsNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetPlatformsNumber", PLATFORMNUMBER, 0 ));
	return 1;	
}

int CLuaGameProject::GetLanguagesNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetLanguagesNumber", LANGNUMBER, 0 ));
	return 1;	
}

int CLuaGameProject::GetScenario( lua_State* L )
{
	lua_pushlightuserdata( L, GetParam_<PNrpScenario>( L, "GetScenario", SCENARIO, NULL ));
	return 1;	
}

int CLuaGameProject::GetLicense( lua_State* L )
{
	lua_pushlightuserdata( L, GetParam_<PNrpScenario>( L, "GetLicense", GLICENSE, NULL ));
	return 1;	
}

int CLuaGameProject::GetGenre( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:GetGenre need int parameter" );

	int idx = lua_tointeger( L, 2 );
	INrpProject* ganre = NULL;

	IF_OBJECT_NOT_NULL_THEN ganre = object_->GetGenre( idx );
	lua_pushlightuserdata( L, ganre );

	return 1;		
}

int CLuaGameProject::GetGenreModuleNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetGenreModuleNumber", GENRE_MODULE_NUMBER, 0 ));
	return 1;			
}

int CLuaGameProject::SetGenre( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaGameProject:SetGenre need INrpProject parameter" );

	INrpProject* gan = (INrpProject*)lua_touserdata( L, 2 );
	int number = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN object_->SetGenre( gan, number );

	return 1;	
}

int CLuaGameProject::GetCodeVolume( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetCodeVolume", CODEVOLUME, 0 ));
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

	IF_OBJECT_NOT_NULL_THEN isIncl = (object_->GetOption<PNrpGameEngine>( GAME_ENGINE ) == ge);
	lua_pushboolean( L, isIncl );

	return 1;		
}

int CLuaGameProject::GetGameEngine( lua_State* L )
{
	lua_pushlightuserdata( L, GetParam_<PNrpGameEngine>( L, "GetGameEngine", GAME_ENGINE, NULL ));
	return 1;	
}

int CLuaGameProject::IsProjectReady( lua_State* L )
{
	lua_pushboolean( L, GetParam_<bool>( L, "IsProjectReady", PROJECTREADY, false ));
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

int CLuaGameProject::GetScriptEngine( lua_State* L )
{
	lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetScriptEngine", SCRIPTENGINE, NULL ) );
	return 1;
}

int CLuaGameProject::SetScriptEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:SetScriptEngine need PNrpTechnology parameter" );
	PNrpTechnology engine = (PNrpTechnology)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->SetOption<PNrpTechnology>( SCRIPTENGINE, engine );

	return 1;		
}

int CLuaGameProject::GetMiniGameEngine( lua_State* L )
{
	lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetMiniGameEngine", MINIGAMEENGINE, NULL ) );
	return 1;
}

int CLuaGameProject::GetPhysicEngine( lua_State* L )
{
	lua_pushlightuserdata( L, GetParam_<PNrpTechnology>( L, "GetPhysicEngine", PHYSICSENGINE, NULL ) );
	return 1;
}

int CLuaGameProject::SetPhysicEngine( lua_State* L)
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:SetPhysicEngine need PNrpTechnology parameter" );
	PNrpTechnology engine = (PNrpTechnology)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->SetOption<PNrpTechnology>( PHYSICSENGINE, engine );

	return 1;	
}

int CLuaGameProject::SetMiniGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:SetMiniGameEngine need PNrpTechnology parameter" );
	PNrpTechnology engine = (PNrpTechnology)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->SetOption<PNrpTechnology>( MINIGAMEENGINE, engine );

	return 1;	
}

int CLuaGameProject::GetAdvTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetAdvTechNumber", ADVTECHNUMBER, 0 ) );
	return 1;
}

int CLuaGameProject::GetAdvTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:GetAdvTech need int parameter" );

	int idx = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;

	IF_OBJECT_NOT_NULL_THEN tech = object_->GetAdvTech( idx );
	lua_pushlightuserdata( L, tech );

	return 1;	
}

int CLuaGameProject::SetAdvTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaGameProject:SetGenre need CNrpTechnology parameter" );

	PNrpTechnology ptrTech = (PNrpTechnology)lua_touserdata( L, 2 );
	int number = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN object_->SetAdvTech( ptrTech, number );
	return 1;	
}

int CLuaGameProject::IsAdvTechInclude( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:IsAdvTechInclude need ADV_TECH_TYPE parameter" );

	ADV_TECH_TYPE techType = ADV_TECH_TYPE( lua_tointeger( L, 2 ) );
	bool isIncl = false;

	IF_OBJECT_NOT_NULL_THEN isIncl = object_->IsAdvTechInclude( techType );
	lua_pushboolean( L, isIncl );

	return 1;			
}
}//namespace nrp