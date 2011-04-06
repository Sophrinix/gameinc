#include "StdAfx.h"
#include "LuaCompany.h"
#include "IUser.h"
#include "NrpCompany.h"
#include "NrpGameEngine.h"
#include "NrpGameProject.h"
#include "NrpGame.h"
#include "NrpApplication.h"
#include "NrpDevelopGame.h"
#include <assert.h>
#include "LuaGame.h"
#include "LuaUser.h"
#include "LuaProject.h"
#include "LuaDevelopProject.h"
#include "LuaInvention.h"
#include "LuaGameEngine.h"
#include "LuaGameProject.h"

namespace nrp
{
CLASS_NAME CLASS_LUACOMPANY( "CLuaCompany" );

BEGIN_LUNA_METHODS(CLuaCompany)
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetEngine )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddBalance )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddGameEngine )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetTech )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, CreateDevelopGame )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetDevProject )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddUser )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, RemoveUser )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetUser )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetProject )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetProjectByName )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddToPortfelle )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetObjectsInPortfelle )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetFromPortfelle )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetGame )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, Create )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, StartInvention )
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetInvention )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaCompany)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaCompany )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "profitLastYear", GetProfitLastYear, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "pieCost", GetPieCost, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "dividend", GetDividend, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "enginesNumber", GetEnginesNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "name", GetName, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "ceo", PureFunction, SetCEO )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "balance", GetBalance, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "techNumber", GetTechNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "devProjectNumber", GetDevProjectNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "userNumber", GetUserNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "projectNumber", GetProjectNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "gameNumber", GetGameNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "inventionNumber", GetInventionNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "allPie", GetAllPie, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "selfPie", GetSelfPie, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaCompany, "texture", GetTexture, PureFunction )
END_LUNA_PROPERTIES

CLuaCompany::CLuaCompany(lua_State *L, bool ex)	: ILuaProject(L, CLASS_LUACOMPANY, ex)	//конструктор
{}

int CLuaCompany::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaCompany:Create need companyname, ceoname parameter" );

	NrpText name = lua_tostring( L, 2 );
	NrpText ceo = lua_tostring( L, 3 );

	PUser user = _nrpApp.GetUser( ceo );
 
	assert( user != NULL );

	if( !user )
	{
		Log(HW) << "не удалось найти пользователя с именем" << ceo << term;
		return 1;
	}
	_object = new CNrpCompany( name, user );
	_nrpApp.AddCompany( _object );

	lua_pushlightuserdata( L, _object );
	return 1;
}

int CLuaCompany::GetAllPie( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, PIE_NUMBER, 0 ) );
	return 1;
}

int CLuaCompany::GetSelfPie( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, SELF_PIE_NUMBER, 0 ) );
	return 1;
}

int CLuaCompany::SetCEO( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpUser* user = _GetLuaObject< CNrpUser, CLuaUser >( L, -1, false );
		(*_object)[ nrp::CEO ] = user;
	}

	return 0;
}

int CLuaCompany::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, NAME, "" ) ); 
	return 1;
}

int CLuaCompany::GetInventionNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, INVENTIONSNUMBER, 0 ) );
	return 1;
}

int CLuaCompany::GetBalance( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, BALANCE, 0 ) );
	return 1;
}

int CLuaCompany::GetEnginesNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, ENGINES_NUMBER, 0 ) );
	return 1;
}

int CLuaCompany::GetEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetEngine need int parameter" );

	int idx = lua_tointeger( L, 2 );
	CNrpGameEngine* eng = NULL;
	IF_OBJECT_NOT_NULL_THEN	eng = _object->GetGameEngine( idx );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, eng );
	Luna< CLuaGameEngine >::constructor( L );

	return 1;
}

int CLuaCompany::AddGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:AddGameEngine need CNrpGameEngine parameter" );

	CNrpGameEngine* ptrGameEng = _GetLuaObject< CNrpGameEngine, ILuaObject >( L, 2, true );

	IF_OBJECT_NOT_NULL_THEN _object->AddGameEngine( ptrGameEng );

	return 1;
}

int CLuaCompany::GetTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, TECHNUMBER, 0 ));
	return 1;
}

int CLuaCompany::GetTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need int parameter" );

	int index = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN	tech = _object->GetTechnology( index );

	lua_pushlightuserdata( L, tech );
	return 1;
}

int CLuaCompany::CreateDevelopGame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need CNrpGameProject* parameter" );

	CNrpGameProject* ptrData = _GetLuaObject< CNrpGameProject, ILuaObject >( L, 2, true);

	assert( ptrData != NULL );

	INrpDevelopProject* result = NULL;
	
	IF_OBJECT_NOT_NULL_THEN	
	{
		if( !_object->GetDevelopProject( (*ptrData)[ NAME ] ) )
		{
			result = new CNrpDevelopGame( ptrData, _object );
			_nrpApp.AddDevelopProject( result );
			_object->AddDevelopProject( result );

			//lua_pop( L, argc );
			lua_pushlightuserdata( L, result );
			Luna< CLuaGameProject >::constructor( L );
		}
	}

	return 1;	
}

int CLuaCompany::AddUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:AddUser need IUser* parameter" );

	CNrpUser* ptrData = _GetLuaObject< CNrpUser, CLuaUser >( L, 2, false );
	assert( ptrData != NULL );

	IF_OBJECT_NOT_NULL_THEN	_object->AddUser( ptrData );

	return 1;		
}

int CLuaCompany::GetUserNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, USERNUMBER, 0 ) ); 
	return 1;
}

int CLuaCompany::GetInvention( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetInvention need int parameter" );

	int index = lua_tointeger( L, 2 );
	CNrpInvention* inv = NULL;
	IF_OBJECT_NOT_NULL_THEN	inv = _object->GetInvention( index );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, inv );
	Luna< CLuaInvention >::constructor( L );

	return 1;
}

int CLuaCompany::GetUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need int parameter" );

	int index = lua_tointeger( L, 2 );
	CNrpUser* user = NULL;
	IF_OBJECT_NOT_NULL_THEN	user = _object->GetUser( index );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, user );
	Luna< CLuaUser >::constructor( L );

	return 1;
}

int CLuaCompany::GetProjectNumber( lua_State *L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, PROJECTNUMBER, 0 ) ); 
	return 1;
}

int CLuaCompany::GetProject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need int parameter" );

	int index = lua_tointeger( L, 2 );
	INrpProject* prj = NULL;
	IF_OBJECT_NOT_NULL_THEN	prj = _object->GetProject( index );

	lua_pushlightuserdata( L, prj );
	return 1;	
}

int CLuaCompany::GetProjectByName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need int parameter" );

	NrpText name( lua_tostring( L, 2 ) );
	INrpProject* prj = NULL;
	IF_OBJECT_NOT_NULL_THEN	prj = _object->GetProject( name );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, prj );
	Luna< CLuaGameProject >::constructor( L );
	return 1;	
}	

int CLuaCompany::AddToPortfelle( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:AddToPortfelle need INrpConfig* parameter" );

	INrpConfig* ptrObject = _GetLuaObject< INrpConfig, ILuaObject >( L, 2, true );

	IF_OBJECT_NOT_NULL_THEN	_object->AddToPortfelle( ptrObject );

	return 1;
}

int CLuaCompany::GetObjectsInPortfelle( lua_State *L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetObjectsInPortfelle", OBJECTSINPORTFELLE, 0 ) ); 
	return 1;
}

int CLuaCompany::GetFromPortfelle( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetFromPortfelle need int parameter" );

	int index = lua_tointeger( L, 2 );
	INrpConfig* prj = NULL;

	IF_OBJECT_NOT_NULL_THEN	prj = _object->GetFromPortfelle( index );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, prj );
	Luna< CLuaProject >::constructor( L );

	return 1;	
}

int CLuaCompany::GetDevProject( lua_State* L )
{	
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetGame need int parameter" );

	int index = lua_tointeger( L, 2 );
	INrpProject* prj = NULL;

	IF_OBJECT_NOT_NULL_THEN	prj = _object->GetDevelopProject( index );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, prj );
	Luna< CLuaDevelopProject >::constructor( L );

	return 1;	
}

int CLuaCompany::GetGame( lua_State* L )
{	
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetGame need int parameter" );

	int index = lua_tointeger( L, 2 );
	INrpConfig* prj = NULL;

	IF_OBJECT_NOT_NULL_THEN	prj = _object->GetGame( index );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, prj );
	Luna< CLuaGame >::constructor( L );

	return 1;	
}

int CLuaCompany::GetGameNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, GAMENUMBER, 0 ) ); 
	return 1;
}

int CLuaCompany::GetDevProjectNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, DEVELOPPROJECTS_NUMBER, 0 ) ); 
	return 1;
}

int CLuaCompany::StartInvention( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:StartInvention need CNrptechnology parameter" );

	NrpText fileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	_nrpApp.AddInvention( fileName, _object );

	return 1;	
}

int CLuaCompany::RemoveUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:StartInvention need CNrptechnology parameter" );

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	_object->RemoveUser( name );

	return 1;		
}

int CLuaCompany::AddBalance( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 2, "Function CLuaCompany:AddBalance need integer parameter" );

	NrpText text = lua_tostring( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	
	{
		//(*_object)[ BOOKKEEPING ].As<CNrpBookKeeping*>()->NewBill( text, valuel, _nrpApp[ CURRENTTIME ].As<NrpTime>() );
		(*_object)[ BALANCE ] += valuel;
	}

	return 1;		
}

const char* CLuaCompany::ClassName()
{
	return ( CLASS_LUACOMPANY );
}

int CLuaCompany::GetProfitLastYear( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		int change = (int)(*_object)[ BALANCE ] - (int)(*_object)[ PROFIT_LASTYEAR ];
		lua_pushinteger( L, change ); 
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaCompany::GetPieCost( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		lua_pushnumber( L, (float)(*_object)[ PIE_COST ] ); 
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaCompany::GetDividend( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		lua_pushnumber( L, (float)(*_object)[ DIVIDEND ] ); 
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaCompany::GetTexture( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		lua_pushstring( L, (NrpText)(*_object)[ TEXTURENORMAL ] ); 
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

}//namespace nrp