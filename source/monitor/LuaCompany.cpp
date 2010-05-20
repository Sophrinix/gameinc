#include "StdAfx.h"
#include "LuaCompany.h"
#include "IUser.h"
#include "NrpCompany.h"
#include "NrpGameEngine.h"
#include "NrpGameProject.h"
#include "NrpGame.h"
#include "NrpApplication.h"
#include <assert.h>

namespace nrp
{

Luna< CLuaCompany >::RegType CLuaCompany::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaCompany ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaCompany, SetCEO ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetName ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetBalance ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetEnginesNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddGameEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetTech ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, CreateGameProject ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddUser ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetUserNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetUser ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetProjectNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetProject ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetProjectByName ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddToPortfelle ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetObjectsInPortfelle ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetFromPortfelle ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetGameNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetGame ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, Create ),
	{0,0}
};

CLuaCompany::CLuaCompany(lua_State *L)	: ILuaProject(L, "CLuaCompany")	//конструктор
{}

int CLuaCompany::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:Create need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );

	object_ = new CNrpCompany( name );
	CNrpApplication::Instance().AddCompany( object_ );

	lua_pushlightuserdata( L, object_ );
	return 1;
}


int CLuaCompany::SetCEO( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:SetCEO need ptrUser parameter" );

	IUser* user = (IUser*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		object_->SetValue<PUser>( nrp::CEO, user );
	}

	return 1;
}

int CLuaCompany::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetName", NAME, "" ).c_str() ); 
	return 1;
}

int CLuaCompany::GetBalance( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetBalance", BALANCE, 0 ) );
	return 1;
}

int CLuaCompany::GetEnginesNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetEnginesNumber", ENGINES_NUMBER, 0 ) );
	return 1;
}

int CLuaCompany::GetEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetEngine need int parameter" );

	int idx = lua_tointeger( L, 2 );
	CNrpGameEngine* eng = NULL;
	IF_OBJECT_NOT_NULL_THEN	eng = object_->GetGameEngine( idx );

	lua_pushlightuserdata( L, eng );
	return 1;
}

int CLuaCompany::AddGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:AddGameEngine need CNrpGameEngine parameter" );

	CNrpGameEngine* ptrGameEng = (CNrpGameEngine*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->AddGameEngine( ptrGameEng );

	return 1;
}

int CLuaCompany::GetTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetTechNumber", TECHNUMBER, 0 ));
	return 1;
}

int CLuaCompany::GetTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need int parameter" );

	int index = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN	tech = object_->GetTechnology( index );

	lua_pushlightuserdata( L, tech );
	return 1;
}

int CLuaCompany::CreateGameProject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need CNrpGameProject* parameter" );

	CNrpGameProject* ptrData = (CNrpGameProject*)lua_touserdata( L, 2 );
	assert( ptrData != NULL );

	CNrpGameProject* result = NULL;
	
	IF_OBJECT_NOT_NULL_THEN	result = object_->AddGameProject( new CNrpGameProject( ptrData, object_ ) );

	lua_pushlightuserdata( L, result );
	return 1;	
}

int CLuaCompany::AddUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:AddUser need IUser* parameter" );

	IUser* ptrData = (IUser*)lua_touserdata( L, 2 );
	assert( ptrData != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->AddUser( ptrData );
	return 1;		
}

int CLuaCompany::GetUserNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetUserNumber", USERNUMBER, 0 ) ); 
	return 1;
}

int CLuaCompany::GetUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need int parameter" );

	int index = lua_tointeger( L, 2 );
	IUser* user = NULL;
	IF_OBJECT_NOT_NULL_THEN	user = object_->GetUser( index );

	lua_pushlightuserdata( L, user );
	return 1;
}

int CLuaCompany::GetProjectNumber( lua_State *L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetProjectNumber", PROJECTNUMBER, 0 ) ); 
	return 1;
}

int CLuaCompany::GetProject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need int parameter" );

	int index = lua_tointeger( L, 2 );
	INrpProject* prj = NULL;
	IF_OBJECT_NOT_NULL_THEN	prj = object_->GetProject( index );

	lua_pushlightuserdata( L, prj );
	return 1;	
}

int CLuaCompany::GetProjectByName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need int parameter" );

	std::string name = lua_tostring( L, 2 );
	INrpProject* prj = NULL;
	IF_OBJECT_NOT_NULL_THEN	prj = object_->GetProject( name );

	lua_pushlightuserdata( L, prj );
	return 1;	
}	

int CLuaCompany::AddLuaFunction( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaCompany:AddLuaFunction need int, string parameter" );

	int typen = lua_tointeger( L, 2 );
	const char* funcName = lua_tostring( L, 3 );
	assert( funcName != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->AddLuaFunction( typen, funcName );

	return 1;
}

int CLuaCompany::AddToPortfelle( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:AddToPortfelle need INrpConfig* parameter" );

	INrpConfig* ptrObject = (INrpConfig*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->AddToPortfelle( ptrObject );

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

	IF_OBJECT_NOT_NULL_THEN	prj = object_->GetFromPortfelle( index );

	lua_pushlightuserdata( L, prj );
	return 1;	
}

int CLuaCompany::GetGame( lua_State* L )
{	
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetGame need int parameter" );

	int index = lua_tointeger( L, 2 );
	INrpConfig* prj = NULL;

	IF_OBJECT_NOT_NULL_THEN	prj = object_->GetGame( index );

	lua_pushlightuserdata( L, prj );
	return 1;	
}

int CLuaCompany::GetGameNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetGameNumber", GAMENUMBER, 0 ) ); 
	return 1;
}

}//namespace nrp