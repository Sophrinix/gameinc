#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpGameProject;
}

namespace nrp
{

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
	int GetScenario( lua_State* L );
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
																
	static const char* StaticGetLuaName() { return "CLuaGameProject"; }
};

}//namespace nrp