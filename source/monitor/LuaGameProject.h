#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpGameProject;
	class CNrpTechnology;
}

namespace nrp
{
class CLuaGameProject : public ILuaProject<nrp::CNrpGameProject>
{
public:
	static Luna<CLuaGameProject>::RegType methods[];				//методы обертки
	static const char* ClassName();

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
	int GetPlatform( lua_State* L );
	int IsMyPlatform( lua_State* L );
	int AddPlatform( lua_State* L );
	int RemovePlatform( lua_State* L );

	int GetLanguagesNumber( lua_State* L );
	int GetLanguage( lua_State* L );
	int IsMyLanguage( lua_State* L );
	int AddLanguage( lua_State* L );
	int RemoveLanguage( lua_State* L );

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
	int GetEngineExtend( lua_State* L );
	int GetLocalization( lua_State* L );
	int GetCrossPlatformCode( lua_State* L );
	int GetCpuUse( lua_State* L );
	int GetMemoryUse( lua_State* L );

	int Create( lua_State* L );
	int Remove( lua_State* L );
															
private:
	int SetNamedTech_( lua_State* L, const NrpText& funcName, const NrpText& paramName );
	
	template< class Param, class T > int SetNumericalTech_( lua_State* L,
											   const NrpText& funcName, 
											   void (T::*Method)( Param* tehc, int index) );

	template< class B, class A, class T > int GetNumericalParam_( lua_State* L,
											   const NrpText& funcName, 
											   A* (T::*Method)( int index) );

	int _TechLuaInitialize( lua_State* L, const NrpText& funcName, OPTION_NAME& paramName );
};

}//namespace nrp