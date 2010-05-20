#include "StdAfx.h"
#include "LuaApplication.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpPlayer.h"
#include "NrpAiUser.h"
#include "nrpEngine.h"
#include "NrpBank.h"
#include "NrpGameProject.h"
#include "NrpTechnology.h"

#include <assert.h>
#include <irrlicht.h>
using namespace irr;

namespace nrp
{

Luna< CLuaApplication >::RegType CLuaApplication::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaApplication ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetCompanyNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetCompany ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetCompanyByName ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, UpdateGameTime ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetBank ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetPlayerCompany ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, RemoveLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetTech ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddPublicTechnology ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetUserNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetUser ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetUserByName ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, RemoveUser ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetCurrentProfile ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetCurrentProfileCompany ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateProfile ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, ResetData ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadProfile ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateNewFreeUsers ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, PayUserSalary ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGameBoxAddonNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGameBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddGameBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadGameBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadGameTimeFromProfile ),
	{0,0}
};

CLuaApplication::CLuaApplication(lua_State *L)	: ILuaProject(L, "CLuaApplication")	//конструктор
{}

int CLuaApplication::UpdateGameTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:UpdateGameTime need 2 parameter" );

	bool dayChange=false, monthChange=false, yearChange=false;
	int idDateLabel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		bool needUpdate = object_->UpdateTime();

		wchar_t text[ 32 ] = { 0 };
		gui::IGUIEnvironment* env = CNrpEngine::Instance().GetGuiEnvironment();
		gui::IGUIElement* elm = env->getRootGUIElement()->getElementFromId( idDateLabel, true );
		SYSTEMTIME& time = object_->GetValue<SYSTEMTIME>( CURRENTTIME );
		if( elm && needUpdate )
		{
			swprintf_s( text, 31, L"%4d.%02d.%02d  %02d:%02d", time.wYear, time.wMonth, time.wDay, 
															   time.wHour, time.wMinute );
			elm->setText( text );
		}

		dayChange = lastDay_ != time.wDay;
		monthChange = lastMonth_ != time.wMonth;
		yearChange = lastYear_ != time.wYear;

		lastDay_ = time.wDay;
		lastMonth_ = time.wMonth;
		lastYear_ = time.wYear;
	}

	return 1;
}

int CLuaApplication::GetBank( lua_State* L )
{
	lua_pushlightuserdata( L, GetParam_<PNrpBank>(L, "GetBank", BANK, NULL ));
	return 1;
}

int CLuaApplication::GetPlayerCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:GetPlayerCompany not need parameter" );

	CNrpCompany* cmp = NULL;
	IF_OBJECT_NOT_NULL_THEN  cmp = object_->GetPlayerCompany();

	lua_pushlightuserdata( L, (void*)cmp );
	return 1;
}

int CLuaApplication::AddRemLuaFunction_( lua_State* L, std::string funcName,  bool rem )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, ("Function CLuaApplication:" + funcName + " need string parameter").c_str() );

	int typea = lua_tointeger( L, 2 );
	const char* fName = lua_tostring( L, 3 );
	assert( fName != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( rem ) object_->RemoveLuaFunction( typea, fName );
		else object_->AddLuaFunction( typea, fName );
	}

#ifdef _DEBUG
	char text[ MAX_PATH ];
	sprintf_s( text, MAX_PATH, "Object:%s %d  FuncName:%s\n", rem ? "remove" : "added", object_, fName );
	OutputDebugString( text );
#endif

	return 1;
}

int CLuaApplication::AddLuaFunction( lua_State* L )
{
	return AddRemLuaFunction_( L, "AddLuaFunction", false );
}

int CLuaApplication::RemoveLuaFunction( lua_State* L )
{
	return AddRemLuaFunction_( L, "RemoveLuaFunction", true );
}

int CLuaApplication::GetTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetTechNumber", TECHNUMBER, 0 ) );
	return 1;
}

int CLuaApplication::GetTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need int parameter" );

	int techNumber = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN	tech = object_->GetTechnology( techNumber );

	lua_pushlightuserdata( L, tech );
	return 1;
}

int CLuaApplication::AddPublicTechnology( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need CNrpTechnology* parameter" );

	CNrpTechnology* tech = (CNrpTechnology*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->AddTechnology( tech );

	return 1;
}

int CLuaApplication::GetUserNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetUserNumber", USERNUMBER, 0 ) );
	return 1;
}

int CLuaApplication::GetUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need int parameter" );

	int userNumber = lua_tointeger( L, 2 );
	IUser* user = NULL;
	IF_OBJECT_NOT_NULL_THEN	user = object_->GetUser( userNumber );

	lua_pushlightuserdata( L, user );
	return 1;
}

int CLuaApplication::GetUserByName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUserByName need string parameter" );

	const char* userName = lua_tostring( L, 2 );
	assert( userName != NULL );
	IUser* user = NULL;
	IF_OBJECT_NOT_NULL_THEN
	{
		int userNum = object_->GetValue<int>( USERNUMBER );
		for( int k = 0; k < userNum; k++ )
		{
			IUser* ptrUser = object_->GetUser( k );
			if( ptrUser->GetValueA<std::string>( NAME ) == std::string( userName ) )
			{
				user = ptrUser;
				break;
			}
		}
	}

	lua_pushlightuserdata( L, user );
	return 1;	
}

int CLuaApplication::RemoveUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:RemoveUser need IUser* parameter" );

	IUser* user = (IUser*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN	object_->RemoveUser( user );
	return 1;	
}

int CLuaApplication::GetCurrentProfile( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetCurrentProfile", PROFILENAME, "" ).c_str() );
	return 1;
}

int CLuaApplication::GetCurrentProfileCompany( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetCurrentProfileCompany", PROFILECOMPANY, "" ).c_str() );
	return 1;
}

int CLuaApplication::CreateProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaCompany:CreateProfile need string,string parameters" );

	const char* userName = lua_tostring( L, 2 );
	assert( userName != NULL );
	const char* companyName = lua_tostring( L, 3 );
	assert( companyName != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->CreateProfile( userName, companyName );
	return 1;	
}

int CLuaApplication::ResetData( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaCompany:ResetData not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN	object_->ResetData();
	return 1;
}

int CLuaApplication::LoadProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaCompany:LoadProfile need string,string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	const char* company = lua_tostring( L, 3 );
	assert( company != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->LoadProfile( name, company );

	return 1;	
}

int CLuaApplication::CreateNewFreeUsers( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:UpdateUsers not need parameter" );

	IF_OBJECT_NOT_NULL_THEN	object_->CreateNewFreeUsers();

	return 1;		
}

int CLuaApplication::PayUserSalary( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:PayUserSalary not need parameter" );

	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpApplication::COMPANIES_LIST& cmpList = object_->GetCompanies();
		//for( size_t cnt=0; cnt < cmpList.size(); cnt++ )
		//	cmpList[ cnt ]->PaySalaries_();
	}

	return 1;
}

int CLuaApplication::GetCompanyNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaCompany:GetCompanyNumber need int parameter" );

	int companyNumber = 0;
	IF_OBJECT_NOT_NULL_THEN	companyNumber = object_->GetValue<int>( COMPANIESNUMBER );

	lua_pushinteger( L, companyNumber );
	return 1;
}

int CLuaApplication::GetCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need int parameter" );

	int cmpNumber = lua_tointeger( L, 2 );
	CNrpCompany* ptrCompany = NULL;

	IF_OBJECT_NOT_NULL_THEN	ptrCompany = object_->GetCompany( cmpNumber );

	lua_pushlightuserdata( L, ptrCompany );
	return 1;
}

int CLuaApplication::GetCompanyByName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetUser need string parameter" );

	const char* cmpName = lua_tostring( L, 2 );
	assert( cmpName != NULL );
	CNrpCompany* ptrCompany = NULL;

	IF_OBJECT_NOT_NULL_THEN	ptrCompany = object_->GetCompany( cmpName );

	lua_pushlightuserdata( L, ptrCompany );
	return 1;
}

int CLuaApplication::GetGameBoxAddonNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaCompany:GetGameBoxAddonNumber not need any parameter" );

	int addonNumber = 0;
	IF_OBJECT_NOT_NULL_THEN	addonNumber = object_->GetBoxAddonsNumber();

	lua_pushinteger( L, addonNumber );
	return 1;
}

int CLuaApplication::GetGameBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetGameBoxAddon need integer parameter" );

	int addonNumber = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN	tech = object_->GetBoxAddon( addonNumber );

	lua_pushlightuserdata( L, tech );
	return 1;	
}

int CLuaApplication::LoadGameBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:LoadGameBoxAddon need string parameter" );

	const char* techIniFile = lua_tostring( L, 2 );
	assert( techIniFile != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpTechnology* tech = object_->CreateTechnology( 0 );
		tech->Load( techIniFile );
		object_->AddBoxAddon( tech );
	}

	return 1;	
}

int CLuaApplication::AddGameBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetGameBoxAddon need integer parameter" );

	CNrpTechnology* tech = (CNrpTechnology*)lua_touserdata( L, 2 );
	assert( tech != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->AddBoxAddon( tech );

	return 1;	
}

int CLuaApplication::LoadGameTimeFromProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:LoadGameTimeFromProfile need profileName parameter" );

	const char* profileName = lua_tostring( L, 2 );
	assert( profileName != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		std::string pathToFile = std::string( "save/" ) + profileName + "/profile.ini";
		
		object_->SetValue<SYSTEMTIME>( CURRENTTIME, IniFile::Read( PROPERTIES, CURRENTTIME + ":time", SYSTEMTIME(), pathToFile ) );
	}

	return 1;	
}
}//namespace nrp 