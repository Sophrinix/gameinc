#include "StdAfx.h"
#include "LuaBank.h"
#include "NrpBank.h"
#include "NrpLoan.h"
#include "NrpCompany.h"
#include <assert.h>

namespace nrp
{

Luna< CLuaBank >::RegType CLuaBank::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaBank ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetMaxCompanyLoan ),
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoansNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanID ),
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanStartSumm ),
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanCompanyName ),
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanMoneyToClose ),
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanMoneyPerMonth ),
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanMonthToEnd ),
	LUNA_AUTONAME_FUNCTION( CLuaBank, CreateLoan ),
	{0,0}
};

CLuaBank::CLuaBank(lua_State *L)	: ILuaProject(L, "CLuaBank")	//конструктор
{}

int CLuaBank::GetMaxCompanyLoan( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaBank::GetMaxCompanyLoan need string parameter");

	const char* companyName = lua_tostring( L, 2 );
	assert( companyName != NULL );
	int sum = 0;
	IF_OBJECT_NOT_NULL_THEN sum = object_->GetMaxCompanyLoan( companyName );

	lua_pushinteger( L, sum );

	return 1;
}

int CLuaBank::GetLoansNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_( L, "GetLoansNumber", LOANNUMBER, 0 ));
	return 1;
}

int CLuaBank::GetLoanID( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaBank:GetLoanID need int parameter");

	int pos = lua_tointeger( L, 2 );
	int id = 0;
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpLoan* loan = object_->GetLoan( pos );
		if( loan )
			id = loan->GetValue<int>( ID  );
	}

	lua_pushinteger( L, id );

	return 1;
}

int CLuaBank::GetLoanCompanyName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaBank:GetLoanCompanyName need int parameter");

	int id = lua_tointeger( L, 2 );
	std::string name = "";
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpLoan* loan = object_->FindLoadByID( id );
		if( loan )
			name = loan->GetValue<PNrpCompany>( COMPANY )->GetValue<std::string>( NAME );
	}

	lua_pushstring( L, name.c_str() );

	return 1;
}

template< class T > T CLuaBank::GetLoanParam_( lua_State* L, std::string funcName, std::string name, T defValue )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, ("Function CLuaBank:" + funcName + " need int parameter").c_str() );

	int id = lua_tointeger( L, 2 );
	T paramd;
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpLoan* loan = object_->FindLoadByID( id );
		if( loan )
			paramd = loan->GetValue<T>( name );
	}

	return paramd;
}

int CLuaBank::GetLoanStartSumm( lua_State* L )
{
	lua_pushinteger( L, GetLoanParam_<int>( L, "GetLoanStartSumm", STARTMONEY, 0 ));
	return 1;
}

int CLuaBank::GetLoanMoneyToClose( lua_State* L )
{
	lua_pushinteger( L, GetLoanParam_<int>( L, "GetLoanMoneyToClose", MONEY, 0 ));
	return 1;
}

int CLuaBank::GetLoanMoneyPerMonth( lua_State* L )
{
	int moneyLeftpay = GetLoanParam_<int>( L, "GetLoanMoneyPerMonth", MONEY, 0 );
	int monthLeft = GetLoanParam_<int>( L, "GetLoanMoneyPerMonth", MONTHLEFT, 0 );
	lua_pushnumber( L, moneyLeftpay / (float)monthLeft );
	return 1;
}

int CLuaBank::GetLoanMonthToEnd( lua_State* L )
{
	lua_pushinteger( L, GetLoanParam_<int>( L, "GetLoanMonthToEnd", MONTHLEFT, 0 ) );
	return 1;
}

int CLuaBank::CreateLoan( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaBank:CreateLoan need 5 parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	int money = lua_tointeger( L, 3 );
	int percent = lua_tointeger( L, 4 );
	int month = lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN	object_->CreateLoan( name, money, percent, month );

	return 1;
}
}//namespace nrp