#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpCompany;
}

namespace nrp
{

class CLuaCompany : public ILuaProject<nrp::CNrpCompany>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaCompany)
	static const char* ClassName();

	CLuaCompany(lua_State *L, bool);		
	int SetCEO( lua_State* L );
	int GetName( lua_State* L );
	int GetBalance( lua_State* L );
	int AddBalance( lua_State* L );
	int GetEnginesNumber( lua_State* L );
	int GetEngine( lua_State* L );
	int AddGameEngine( lua_State* L );
	int GetTechNumber( lua_State* L );
	int GetTech( lua_State* L );
	int CreateDevelopGame( lua_State* L );
	int GetDevProjectNumber( lua_State* L );
	int GetProjectNumber( lua_State *L );
	int GetUserNumber( lua_State* L );
	int GetProject( lua_State* L );
	int GetProjectByName( lua_State* L );
	int AddUser( lua_State* L );
	int RemoveUser( lua_State* L );
	int RemoveTech( lua_State* L );
	int GetUser( lua_State* L );
	int AddToPortfelle( lua_State* L );
	int GetObjectsInPortfelle( lua_State* L );
	int GetFromPortfelle( lua_State* L );
	int GetGameNumber( lua_State* L );
	int GetGame( lua_State* L );
	int Create( lua_State* L );
	int GetDevProject( lua_State* L );
	int StartInvention( lua_State* L );
	int GetProfitLastYear( lua_State* L );
	int GetPieCost( lua_State* L );
	int GetDividend( lua_State* L );
	int GetTexture( lua_State* L );

	int GetInventionNumber( lua_State* L );
	int GetInvention( lua_State* L );

	int GetAllPie( lua_State* L );
	int GetSelfPie( lua_State* L );
	int SetInvExp( lua_State* L );
	int GetInvExp( lua_State* L );
    int GetLastMonthSales( lua_State* L );
    int GetAllTimeSales( lua_State* L );
    int GetLastMonthProfit( lua_State* L );
    int SetSelfPie( lua_State* L );
    int SetAllPie( lua_State* L );
    int SetTexture( lua_State* L );
    int AddGame( lua_State* L );
};

}//namespace nrp