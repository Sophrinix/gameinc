#include "StdAfx.h"
#include "LuaBank.h"
#include "NrpBank.h"
#include "NrpLoan.h"
#include "NrpCompany.h"
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_LUABANK( "CLuaBank" );

BEGIN_LUNA_METHODS(CLuaBank)
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetMaxCompanyLoan )
	
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanID )
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanStartSumm )
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanCompanyName )
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanMoneyToClose )
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanMoneyPerMonth )
	LUNA_AUTONAME_FUNCTION( CLuaBank, GetLoanMonthToEnd )
	LUNA_AUTONAME_FUNCTION( CLuaBank, CreateLoan )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaBank)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaBank )
	LUNA_AUTONAME_PROPERTY( CLuaBank, "loansNumber", GetLoansNumber, PureFunction )
END_LUNA_PROPERTIES

CLuaBank::CLuaBank(lua_State *L, bool ex)	: ILuaProject(L, CLASS_LUABANK, ex)	//конструктор
{}

int CLuaBank::GetMaxCompanyLoan( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaBank::GetMaxCompanyLoan need string parameter");

	NrpText companyName = lua_tostring( L, 2 );

	int sum = 0;
	IF_OBJECT_NOT_NULL_THEN sum = _object->GetMaxCompanyLoan( companyName );

	lua_pushinteger( L, sum );

	return 1;
}

int CLuaBank::GetLoansNumber( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, GetParam_( L, PROP, LOANNUMBER, 0 ));
		return  1;
	}

	lua_pushnil( L );
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
		CNrpLoan* loan = _object->GetLoan( pos );
		if( loan )
			id = (*loan)[ ID ];
	}

	lua_pushinteger( L, id );

	return 1;
}

int CLuaBank::GetLoanCompanyName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaBank:GetLoanCompanyName need int parameter");

	int id = lua_tointeger( L, 2 );
	NrpText name( "" );
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpLoan* loan = _object->FindLoadByID( id );
		if( loan )
			name = (NrpText)(*loan)[ COMPANYNAME ];
	}

	lua_pushstring( L, name );

	return 1;
}

template< class T > T CLuaBank::GetLoanParam_( lua_State* L, NrpText& funcName, OPTION_NAME& name, T defValue )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, _ErrStr(funcName + " need int parameter") );

	int id = lua_tointeger( L, 2 );
	T paramd;
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpLoan* loan = _object->FindLoadByID( id );
		if( loan )
			paramd = (*loan)[ name ].As<T>();
	}

	return paramd;
}

int CLuaBank::GetLoanStartSumm( lua_State* L )
{
	lua_pushinteger( L, GetLoanParam_<int>( L, NrpText("GetLoanStartSumm"), STARTMONEY, 0 ));
	return 1;
}

int CLuaBank::GetLoanMoneyToClose( lua_State* L )
{
	lua_pushinteger( L, GetLoanParam_<int>( L, NrpText("GetLoanMoneyToClose"), MONEY, 0 ));
	return 1;
}

int CLuaBank::GetLoanMoneyPerMonth( lua_State* L )
{
	int moneyLeftpay = GetLoanParam_<int>( L, NrpText("GetLoanMoneyPerMonth"), MONEY, 0 );
	int monthLeft = GetLoanParam_<int>( L, NrpText("GetLoanMoneyPerMonth"), MONTHLEFT, 0 );
	lua_pushnumber( L, moneyLeftpay / (float)monthLeft );
	return 1;
}

int CLuaBank::GetLoanMonthToEnd( lua_State* L )
{
	lua_pushinteger( L, GetLoanParam_<int>( L, NrpText("GetLoanMonthToEnd"), MONTHLEFT, 0 ) );
	return 1;
}

int CLuaBank::CreateLoan( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaBank:CreateLoan need 5 parameter" );

	NrpText name = lua_tostring( L, 2 );

	int money = lua_tointeger( L, 3 );
	int percent = lua_tointeger( L, 4 );
	int month = lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN	_object->CreateLoan( name, money, percent, month );

	return 1;
}

const char* CLuaBank::ClassName()
{
	return ( CLASS_LUABANK );
}
}//namespace nrp