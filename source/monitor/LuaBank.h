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
	DEFINE_PROPERTIES_AND_METHODS(CLuaBank)
	static const char* ClassName();

	CLuaBank(lua_State *L, bool );		
	int GetMaxCompanyLoan( lua_State* L );
	int GetLoansNumber( lua_State* L );
	int GetLoanID( lua_State* L );
	int GetLoanCompanyName( lua_State* L );
	int GetLoanStartSumm( lua_State* L );
	int GetLoanMoneyToClose( lua_State* L );
	int GetLoanMoneyPerMonth( lua_State* L );
	int GetLoanMonthToEnd( lua_State* L );
	int CreateLoan( lua_State* L );

private:
	template< class T > T GetLoanParam_( lua_State* L, NrpText& funcName, OPTION_NAME& name, T defValue );
};

}//namespace nrp