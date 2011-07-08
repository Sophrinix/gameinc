#pragma once
#include "ILuaProject.h"

namespace nrp
{

class CLuaPlatform : public ILuaProject<nrp::CNrpPlatform>
{
public:
    static const int defaultSale = 1000000;
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaPlatform)
	static const char* ClassName();

	CLuaPlatform(lua_State *L, bool );		

	int Load( lua_State* L );
	int Create( lua_State* L );
	int GetCpu( lua_State* L );
	int GetRam( lua_State* L );
	int IsMyTech( lua_State* L );
    int CreateRandomSales( lua_State* L );
    int GetLastMonthProfit( lua_State* L );
    int GetLastMonthSales( lua_State* L );
    int GetAllTimeSales( lua_State* L );
    int GetPrice( lua_State* L );
    int SetPrice( lua_State* L );
    int GetAllTimeProfit( lua_State* L );
    int CreateSalesHistory( lua_State* L );
};

}//end namespace nrp