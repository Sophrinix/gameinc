#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpBank;
}

namespace nrp
{

class CLuaBank : public ILuaProject<nrp::CNrpBank>
{
public:
	static Luna<CLuaBank>::RegType methods[];				//методы обертки

	CLuaBank(lua_State *L);		
	int GetMaxCompanyLoan( lua_State* L );
	int GetLoansNumber( lua_State* L );
	int GetLoanID( lua_State* L );
	int GetLoanCompanyName( lua_State* L );
	int GetLoanStartSumm( lua_State* L );
	int GetLoanMoneyToClose( lua_State* L );
	int GetLoanMoneyPerMonth( lua_State* L );
	int GetLoanMonthToEnd( lua_State* L );
	int CreateLoan( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaBank"; }
private:
	template< class T > T GetLoanParam_( lua_State* L, std::string funcName, std::string name, T defValue );
};

}//namespace nrp