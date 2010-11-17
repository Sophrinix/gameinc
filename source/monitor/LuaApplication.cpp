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
#include "NrpScreenshot.h"
#include "NrpGameTime.h"

#include "LuaCompany.h"
#include "LuaUser.h"
#include "LuaBank.h"
#include "LuaTechnology.h"
#include "LuaInvention.h"
#include "LuaPda.h"
#include "LuaGame.h"
#include "OpFileSystem.h"
#include "IniFile.h"

#include <assert.h>
#include <irrlicht.h>
#include <io.h>
using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_CLUAPPLICATION( "CLuaApplication" );

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
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGameBoxAddonNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGameBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddGameBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadGameBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadGameTimeFromProfile ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, SaveBoxAddonsPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadBoxAddonsPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGamesNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGame ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddGameToMarket ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadScreenshots ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGameTime ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetInvention ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateDirectorySnapshot ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetPda ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetPauseBetweenStep ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, SetPauseBetweenStep ),
	{0,0}
};

CLuaApplication::CLuaApplication(lua_State *L)	: ILuaProject(L, CLASS_CLUAPPLICATION )	//конструктор
{}

int CLuaApplication::UpdateGameTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:UpdateGameTime need 2 parameter" );

	int idDateLabel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		bool needUpdate = object_->GetValue<CNrpGameTime*>( GAME_TIME )->Update();

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
	}

	return 1;
}

int CLuaApplication::GetBank( lua_State* L )
{
	PNrpBank bank = GetParam_<PNrpBank>(L, "GetBank", BANK, NULL );
	lua_pop( L, lua_gettop( L ) );
	lua_pushlightuserdata( L, bank );
	Luna< CLuaBank >::constructor( L );
	return 1;
}

int CLuaApplication::GetPlayerCompany( lua_State* L )
{
	PNrpCompany cmp = GetParam_<PNrpCompany>( L, "GetPlayerCompany", PLAYERCOMPANY, NULL );
	lua_pop( L, lua_gettop( L ) );
	lua_pushlightuserdata( L, cmp );
	Luna< CLuaCompany >::constructor( L );
	return 1;
}

int CLuaApplication::AddRemLuaFunction_( lua_State* L, const NrpText& funcName,  bool rem )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, _ErrStr(funcName + " need string parameter") );

	int typea = lua_tointeger( L, 2 );
	NrpText fName = lua_tostring( L, 3 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( rem ) object_->RemoveLuaFunction( typea, fName );
		else object_->AddLuaFunction( typea, fName );
	}

#ifdef _DEBUG
	Log(HW) << "Object:" << NrpText(rem ? "remove" : "added") << (int)object_ << fName << term;
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
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetTech need int parameter" );

	CNrpTechnology* tech = NULL;

	IF_OBJECT_NOT_NULL_THEN	
	{
		if( lua_isnumber( L, 2 ) > 0 )
		{
			int techNumber = lua_tointeger( L, 2 );
			tech = object_->GetTechnology( techNumber );
		}
		else if( lua_isstring( L, 2 ) > 0 )
		{
			NrpText name( lua_tostring( L, 2 ) );
			tech = object_->GetTechnology( name );	
		}
	}

	lua_pop( L, argc );
	lua_pushlightuserdata( L, tech );
	Luna< CLuaTechnology >::constructor( L );

	return 1;
}

int CLuaApplication::AddPublicTechnology( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetTech need CNrpTechnology* parameter" );

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
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetUser need int parameter" );

	int userNumber = lua_tointeger( L, 2 );
	IUser* user = NULL;
	IF_OBJECT_NOT_NULL_THEN	user = object_->GetUser( userNumber );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, user );
	Luna< CLuaUser >::constructor( L );
	return 1;
}

int CLuaApplication::GetUserByName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetUserByName need string parameter" );

	NrpText userName = lua_tostring( L, 2 );

	IUser* user = NULL;
	IF_OBJECT_NOT_NULL_THEN
	{
		int userNum = object_->GetValue<int>( USERNUMBER );
		for( int k = 0; k < userNum; k++ )
		{
			IUser* ptrUser = object_->GetUser( k );
			if( ptrUser->GetString( NAME ).equals_ignore_case( userName ) )
			{
				user = ptrUser;
				break;
			}
		}
	}

	lua_pop( L, argc );
	lua_pushlightuserdata( L, user );
	Luna< CLuaUser >::constructor( L );
	return 1;	
}

int CLuaApplication::RemoveUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:RemoveUser need IUser* parameter" );

	IUser* user = (IUser*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN	object_->RemoveUser( user );
	return 1;	
}

int CLuaApplication::GetCurrentProfile( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetCurrentProfile", PROFILENAME, "" ) );
	return 1;
}

int CLuaApplication::GetCurrentProfileCompany( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetCurrentProfileCompany", PROFILECOMPANY, "" ) );
	return 1;
}

int CLuaApplication::CreateProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:CreateProfile need string,string parameters" );

	NrpText userName( lua_tostring( L, 2 ) );
	NrpText companyName( lua_tostring( L, 3 ) );

	IF_OBJECT_NOT_NULL_THEN	object_->CreateProfile( userName, companyName );
	return 1;	
}

int CLuaApplication::ResetData( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:ResetData not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN	object_->ResetData();
	return 1;
}

int CLuaApplication::LoadProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:LoadProfile need string,string parameter" );

	NrpText name( lua_tostring( L, 2 ) );
	NrpText company( lua_tostring( L, 3 ) );

	IF_OBJECT_NOT_NULL_THEN	object_->Load( name, company );

	return 1;	
}

int CLuaApplication::CreateNewFreeUsers( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:UpdateUsers not need parameter" );

	IF_OBJECT_NOT_NULL_THEN	object_->CreateNewFreeUsers();

	return 1;		
}

int CLuaApplication::GetCompanyNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:GetCompanyNumber need int parameter" );

	int companyNumber = 0;
	IF_OBJECT_NOT_NULL_THEN	companyNumber = object_->GetValue<int>( COMPANIESNUMBER );

	lua_pushinteger( L, companyNumber );
	return 1;
}

int CLuaApplication::GetCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetUser need int parameter" );

	int cmpNumber = lua_tointeger( L, 2 );
	CNrpCompany* ptrCompany = NULL;

	IF_OBJECT_NOT_NULL_THEN	ptrCompany = object_->GetCompany( cmpNumber );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, ptrCompany );
	Luna< CLuaCompany >::constructor( L );
	return 1;
}

int CLuaApplication::GetCompanyByName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetUser need string parameter" );

	NrpText cmpName( lua_tostring( L, 2 ) );
	CNrpCompany* ptrCompany = NULL;

	IF_OBJECT_NOT_NULL_THEN	ptrCompany = object_->GetCompany( cmpName );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, ptrCompany );
	Luna< CLuaCompany >::constructor( L );
	return 1;
}

int CLuaApplication::GetGameBoxAddonNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:GetGameBoxAddonNumber not need any parameter" );

	int addonNumber = 0;
	IF_OBJECT_NOT_NULL_THEN	addonNumber = object_->GetValue<int>( BOXADDONNUMBER );

	lua_pushinteger( L, addonNumber );
	return 1;
}

int CLuaApplication::GetGameBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetGameBoxAddon need integer parameter" );

	int addonNumber = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN	tech = object_->GetBoxAddon( addonNumber );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, tech );
	Luna< CLuaTechnology >::constructor( L );
	return 1;	
}

int CLuaApplication::LoadGameBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:LoadGameBoxAddon need string parameter" );

	NrpText techIniFile( lua_tostring( L, 2 ) );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpTechnology* tech = new CNrpTechnology( techIniFile );
		ret = object_->AddBoxAddon( tech );
	}

	lua_pushboolean( L, ret );
	return 1;	
}

int CLuaApplication::AddGameBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetGameBoxAddon need integer parameter" );

	CNrpTechnology* tech = (CNrpTechnology*)lua_touserdata( L, 2 );
	assert( tech != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->AddBoxAddon( tech );

	return 1;	
}

int CLuaApplication::LoadGameTimeFromProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:LoadGameTimeFromProfile need profileName parameter" );

	NrpText profileName( lua_tostring( L, 2 ));

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText pathToFile = object_->GetString( SAVEDIR ) + profileName + "/profile.ini";
		
		IniFile rv( pathToFile );
		object_->SetValue<SYSTEMTIME>( CURRENTTIME, rv.Get( SECTION_PROPERTIES, CURRENTTIME + ":time", SYSTEMTIME() ) );
	}

	return 1;	
}

int CLuaApplication::LoadBoxAddonsPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:LoadBoxAddonsPrice not need any parameter" );

	NrpText boxAddonsIni = object_->GetString( SAVEDIR ) + object_->GetString(PROFILENAME)+ "/boxaddons.ini";
	IniFile rv( boxAddonsIni );

	for( int k=0; k < object_->GetValue<int>( BOXADDONNUMBER ); k++ )
	{
		CNrpTechnology* tech = object_->GetBoxAddon( k );
		float price = rv.Get( "prices", tech->GetString( NAME ), tech->GetValue<float>( PRICE ) );
		tech->SetValue( PRICE, price );
	}

	return 1;
}

int CLuaApplication::SaveBoxAddonsPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:SaveBoxAddonsPrice not need any parameter" );

	NrpText boxAddonsIni = object_->GetString( SAVEDIR ) + object_->GetString(PROFILENAME)+ "/boxaddons.ini";
	IniFile sv( boxAddonsIni );
	for( int k=0; k < object_->GetValue<int>( BOXADDONNUMBER ); k++ )
	{
		CNrpTechnology* tech = object_->GetBoxAddon( k );
		sv.Set( "prices", tech->GetString( NAME ), tech->GetValue<float>( PRICE ) );
	}

	return 1;
}

int CLuaApplication::GetGamesNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetGamesNumber", GAMENUMBER, 0 ) );
	return 1;
}

int CLuaApplication::GetGame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetGame need integer(string) parameter" );

	CNrpGame* game = NULL;
	if( lua_isnumber( L, 2 ) )
	{
		int index = lua_tointeger( L, 2 );
		assert( index >= 0 );
		IF_OBJECT_NOT_NULL_THEN	game = object_->GetGame( index );
	}
	else if( lua_isstring( L, 2 ) )
	{
		NrpText name = lua_tostring( L, 2 );
		IF_OBJECT_NOT_NULL_THEN game = object_->GetGame( name );
	}

	lua_pop( L, argc );
	lua_pushlightuserdata( L, game );
	Luna< CLuaGame >::constructor( L );
	return 1;		
}

int CLuaApplication::AddGameToMarket( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:AddGameToMarket need CnrpGame* parameter" );

	CNrpGame* game = (CNrpGame*)lua_touserdata( L, 2 );
	assert( game != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->AddGameToMarket( game );

	return 1;	
}

int CLuaApplication::LoadScreenshots( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:LoadScreenshots need string parameter" );

	NrpText imageListName( lua_tostring( L, 2 ) );
	if( !imageListName.size() )
		return 1;

	IF_OBJECT_NOT_NULL_THEN object_->LoadScreenshots( imageListName );

	return 1;		
}

int CLuaApplication::GetGameTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:GetGameTime not need any parameter" );

	SYSTEMTIME time;
	IF_OBJECT_NOT_NULL_THEN time = object_->GetValue<SYSTEMTIME>( CURRENTTIME );

	lua_pushinteger( L, time.wYear );
	lua_pushinteger( L, time.wMonth );
	lua_pushinteger( L, time.wDay );
	lua_pushinteger( L, time.wHour );
	lua_pushinteger( L, time.wMinute );

	return 5;		
}

int CLuaApplication::GetInvention( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:GetInvention need invention name, company name parameters" );

	NrpText inventionName( lua_tostring( L, 2 ) );
	NrpText companyName( lua_tostring( L, 3 ) );
	CNrpInvention* inv = NULL;

	IF_OBJECT_NOT_NULL_THEN inv = object_->GetInvention( inventionName, companyName );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, inv );
	Luna< CLuaInvention >::constructor( L );
	return 1;
}

int CLuaApplication::CreateDirectorySnapshot( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaApplication:CreateDirectorySnapshot need directory, saveFile, itemName, itemTemplate in parameters" );

	NrpText directory( lua_tostring( L, 2 ) );
	NrpText saveFile( lua_tostring( L, 3 ) );
	NrpText itemTemplate( lua_tostring( L, 4 ) );
	NrpText itemName( lua_tostring( L, 5 ) );

	OpFileSystem::CreateDirectorySnapshot( directory, saveFile, itemTemplate, itemName );
	return 1;
}

int CLuaApplication::GetPda( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:GetGameTime not need any parameter" );

	CNrpPda* pda = NULL;
	IF_OBJECT_NOT_NULL_THEN pda = object_->GetValue<CNrpPda*>( PDA );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, pda );
	Luna< CLuaPda >::constructor( L );

	return 1;		
}

int CLuaApplication::GetPauseBetweenStep( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:GetPauseBetweenStep not need any parameter" );

	int pause = 0;
	IF_OBJECT_NOT_NULL_THEN pause = object_->GetValue<int>( PAUSEBTWSTEP );
	
	lua_pushinteger( L, pause );

	return 1;	
}

int CLuaApplication::SetPauseBetweenStep( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:SetPauseBetweenStep need integer parameter" );

	int pause = lua_tointeger( L, 2 );
	assert( pause > 0  );
	if( pause <= 0 )
		return 1;

	IF_OBJECT_NOT_NULL_THEN object_->SetValue<int>( PAUSEBTWSTEP, pause );

	return 1;	
}

const char* CLuaApplication::ClassName()
{
	return ( CLASS_CLUAPPLICATION );
}
}//namespace nrp  