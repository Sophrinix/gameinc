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
	static Luna<CLuaApplication>::RegType methods[];				//методы обертки

	CLuaApplication(lua_State *L);		

	int CreateCompany( lua_State* L);
	int GetCompanyNumber( lua_State* L );
	int GetCompany( lua_State* L );
	int GetCompanyByName( lua_State* L );

	int CreateUser( lua_State *L );
	int UpdateGameTime( lua_State* L );
	int CreateGameEngine( lua_State* L );
	int GetBank( lua_State* L );
	int GetPlayerCompany( lua_State* L );
	int RemoveLuaFunction( lua_State* L );
	int AddLuaFunction( lua_State* L );
	int CreateGameProject( lua_State* L );
	int CreateTechnology( lua_State* L );
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
	int PayUserSalary( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaApplication"; }
private:
	int lastDay_, lastMonth_, lastYear_;
	int AddRemLuaFunction_( lua_State* L, std::string funcName, bool rem );
	
};

}//namespace nrp