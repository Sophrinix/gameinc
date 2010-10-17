#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpGameProject;
	class CNrpTechnology;
}

namespace nrp
{
CLASS_NAME CLASS_LUAGMEPROJECT( "CLuaGameProject" );

class CLuaGameProject : public ILuaProject<nrp::CNrpGameProject>
{
public:
	static Luna<CLuaGameProject>::RegType methods[];				//методы обертки

	CLuaGameProject(lua_State *L);		
	int SetGameEngine( lua_State* L);
	int GetGenre( lua_State* L ); 
	int GetGenreModuleNumber( lua_State* L );
	int SetGenre( lua_State* L );
	int IsGenreIncluded( lua_State* L );
	int IsMyGameEngine( lua_State* L );
	int GetGameEngine( lua_State* L );
	int GetCodeVolume( lua_State* L );
	int GetCodeQuality( lua_State* L );
	int GetScenario( lua_State* L );
	int SetScenario( lua_State* L );
	int GetLicense( lua_State* L );
	int GetPlatformsNumber( lua_State* L );
	int GetLanguagesNumber( lua_State* L );
	int IsProjectReady( lua_State* L );
	int HaveLicense( lua_State* L );
	int HaveScenario( lua_State* L );
	int SetScriptEngine( lua_State* L );
	int GetScriptEngine( lua_State* L );
	int GetMiniGameEngine( lua_State* L );
	int SetMiniGameEngine( lua_State* L );
	int GetPhysicEngine( lua_State* L );
	int SetPhysicEngine( lua_State* L);
	int GetAdvTechNumber( lua_State* L );
	int IsTechInclude( lua_State* L );
	int GetAdvTech( lua_State* L );
	int SetAdvTech( lua_State* L );
	int GetVideoQuality( lua_State* L );
	int SetVideoQuality( lua_State* L );
	int GetVideoTech( lua_State* L );
	int SetVideoTech( lua_State* L );
	int GetVideoTechNumber( lua_State* L );
	int GetSoundTech( lua_State* L );
	int SetSoundTech( lua_State* L );
	int GetSoundTechNumber( lua_State* L );
	int GetSoundQuality( lua_State* L );
	int SetSoundQuality( lua_State* L );
	int IsLangAvaible( lua_State* L );
	int ToggleLanguage( lua_State* L );
	int IsPlatformAvaible( lua_State* L );
	int TogglePlatform( lua_State* L );
	int GetEngineExtend( lua_State* L );
	int GetLocalization( lua_State* L );
	int GetCrossPlatformCode( lua_State* L );
	int Create( lua_State* L );
	int Remove( lua_State* L );
																
	static const char* ClassName() { return CLASS_LUAGMEPROJECT.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUAGMEPROJECT; }
private:
	int SetNamedTech_( lua_State* L, std::string funcName, const std::string paramName );
	
	template< class T > int SetNumericalTech_( lua_State* L,
											   std::string funcName, 
											   void (T::*Method)( CNrpTechnology* tehc, int index) );

	template< class T > int GetNumericalTech_( lua_State* L,
											   std::string funcName, 
											   CNrpTechnology* (T::*Method)( int index) );

	int IsParamAvaible_( lua_State* L, std::string funcName, std::string prefix );
	int ToggleParam_( lua_State* L, std::string funcName, std::string prefix );
	int _TechLuaInitialize( lua_State* L, std::string funcName, std::string paramName );
};

}//namespace nrp