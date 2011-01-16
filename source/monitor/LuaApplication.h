#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpApplication;
	class CNrpBank;
}

namespace nrp
{

class CLuaApplication : public ILuaProject<nrp::CNrpApplication>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaApplication)
	static const char* ClassName();

	CLuaApplication(lua_State *L, bool ex);		

	int GetCompanyNumber( lua_State* L );
	int GetCompany( lua_State* L );
	int GetCompanyByName( lua_State* L );

	int UpdateGameTime( lua_State* L );
	int GetBank( lua_State* L );
	int GetPlayerCompany( lua_State* L );

	int GetPlatformNumber( lua_State* L );
	int GetPlatform( lua_State* L );
	int AddPlatform( lua_State* L );
	int LoadPlatform( lua_State* L );

	int RemoveLuaFunction( lua_State* L );
	int AddLuaFunction( lua_State* L );
	int GetTechNumber( lua_State* L );
	int GetTech( lua_State* L );
	int AddPublicTechnology( lua_State* L );
	int GetUserNumber( lua_State* L );
	int GetUser( lua_State* L );
	int GetUserByName( lua_State* L );
	int RemoveUser( lua_State* L );
	int GetCurrentProfile( lua_State* L );
	int GetCurrentProfileCompany( lua_State* L);
	int CreateProfile( lua_State* L );
	int ResetData( lua_State* L );
	int LoadProfile( lua_State* L );
	int CreateNewFreeUsers( lua_State* L );
	int GetGameBoxAddonNumber( lua_State* L );
	int GameBoxLoaded( lua_State* L );
	int GetGameBoxAddon( lua_State* L );
	int AddGameBoxAddon( lua_State* L );
	int LoadGameBoxAddon( lua_State* L );
	int LoadGameTimeFromProfile( lua_State* L );
	int SaveBoxAddonsPrice( lua_State* L );
	int LoadBoxAddonsPrice( lua_State* L );
	int GetGamesNumber( lua_State* L );
	int GetGame( lua_State* L );
	int AddGameToMarket( lua_State* L );
	int LoadScreenshots( lua_State* L );
	int GetGameTime( lua_State* L );
	int GetInvention( lua_State* L );
	int GetPda( lua_State* L );

	int CreateDirectorySnapshot( lua_State* L );

	int GetPauseBetweenStep( lua_State* L );
	int SetPauseBetweenStep( lua_State* L );

private:
	int AddRemLuaFunction_( lua_State* L, const NrpText& funcName, bool rem );
};

}//namespace nrp