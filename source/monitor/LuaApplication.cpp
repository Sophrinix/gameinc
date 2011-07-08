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
#include "NrpExtInfo.h"
#include "NrpGameTime.h"
#include "NrpPlatform.h"
#include "NrpPlant.h"
#include "NrpBridge.h"
#include "NrpSoundEngine.h"
#include "NrpQuestEngine.h"
#include "NrpLaborMarket.h"
#include "NrpLinkHolder.h"
#include "NrpScreenshotHolder.h"
#include "NrpGameMarket.h"

#include "LuaCompany.h"
#include "LuaUser.h"
#include "LuaBank.h"
#include "LuaTechnology.h"
#include "LuaInvention.h"
#include "LuaPda.h"
#include "LuaGame.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "LuaPlatform.h"
#include "LuaTechnology.h"
#include "LuaPlant.h"
#include "lualabel.h"
#include "LuaSoundEngine.h"
#include "LuaQuestEngine.h"

#include <assert.h>
#include <irrlicht.h>
#include <io.h>
using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_CLUAPPLICATION( "CLuaApplication" );

BEGIN_LUNA_METHODS(CLuaApplication)
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetCompany )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, SinceCompany )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetCompanyByName )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, Update )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetPlatform )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadPlatform )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, Bind )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, Unbind )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetTech )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddPublicTechnology )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetUser )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetUserByName )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, RemoveUser )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetCurrentProfileCompany )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateProfile )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, ResetData )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadProfile )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateNewFreeUsers )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGameBoxAddon )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddGameBoxAddon )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadGameBoxAddon )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadGameTimeFromProfile )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, SaveBoxAddonsPrice )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadBoxAddonsPrice )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGame )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddGameToMarket )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadScreenshots )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetGameTime )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetInvention )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateDirectorySnapshot )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, LoadLinks )
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddResourceDirectory )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaApplication)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaApplication )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "playerCompany", GetPlayerCompany, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "bank", GetBank, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "companyNumber", GetCompanyNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "platformNumber", GetPlatformNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "techNumber", GetTechNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "userNumber", GetUserNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "profile", GetCurrentProfile, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "boxAddonNumber", GetGameBoxAddonNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "gamesNumber", GetGamesNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "pda", GetPda, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "speed", GetPauseBetweenStep, SetPauseBetweenStep )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "plant", GetPlant, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "difficult", PureFunction, SetDevForce )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "soundEngine", GetSoundEngine, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaApplication, "questEngine", GetQuestEngine, PureFunction )
END_LUNA_PROPERTIES

CLuaApplication::CLuaApplication(lua_State *L, bool ex)	: ILuaProject(L, CLASS_CLUAPPLICATION, ex )	//конструктор
{}

int CLuaApplication::AddResourceDirectory( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:AddResourceDirectory need string parameter" );

	NrpText pathTo = lua_tostring( L, 2 );
	
	assert( OpFileSystem::IsExist( pathTo ) );
	_nrpEngine.GetVideoDriver()->addResourceDirectory( OpFileSystem::RemoveEndSlash( pathTo ) );
	return 1;
}

int CLuaApplication::GetPlant( lua_State* L )
{
	lua_pushlightuserdata( L, (void*)&CNrpPlant::Instance() );
	Luna< CLuaPlant >::constructor( L );
	return 1;
}

int CLuaApplication::Update( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:Update need 2 parameter" );

	IF_OBJECT_NOT_NULL_THEN (*_object)[ GAME_TIME ].As<CNrpGameTime*>()->Update();

	return 0;
}

int CLuaApplication::GetBank( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		//lua_pop( L, lua_gettop( L ) );
		lua_pushlightuserdata( L, &(CNrpBank::Instance()) );
		Luna< CLuaBank >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetPlayerCompany( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		//lua_pop( L, lua_gettop( L ) );
		lua_pushlightuserdata( L, (*_object)[ PLAYERCOMPANY ].As<PNrpCompany>() );
		Luna< CLuaCompany >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::AddRemLuaFunction_( lua_State* L, const NrpText& funcName,  bool rem )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, _ErrStr(funcName + " need string parameter") );

	int typea = lua_tointeger( L, 2 );
	int refFunc = _GetRef( L, 3 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( rem ) _object->Unbind( typea, refFunc );
		else _object->Bind( typea, refFunc );

#ifdef _DEBUG
		Log(HW) << NrpText(rem ? "remove " : "added ") << "application:" << refFunc << term;
#endif
	}

	return 0;
}

int CLuaApplication::Bind( lua_State* L )
{
	return AddRemLuaFunction_( L, "Bind", false );
}

int CLuaApplication::Unbind( lua_State* L )
{
	return AddRemLuaFunction_( L, "Unbind", true );
}

int CLuaApplication::GetTechNumber( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, GetParam_<int>( L, PROP, TECHNUMBER, 0 ) );
		return 1;
	}

	lua_pushnil( L ); 
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
			tech = _object->GetTechnology( techNumber );
		}
		else if( lua_isstring( L, 2 ) > 0 )
		{
			NrpText name( lua_tostring( L, 2 ) );
			tech = _object->GetTechnology( name );	
		}

		//lua_pop( L, argc );
		if( tech )
		{
			lua_pushlightuserdata( L, tech );
			Luna< CLuaTechnology >::constructor( L );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::AddPublicTechnology( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetTech need CNrpTechnology* parameter" );

	CNrpTechnology* tech = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2, false );

	IF_OBJECT_NOT_NULL_THEN	_object->AddTechnology( tech );

	return 0;
}

int CLuaApplication::GetUserNumber( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, _nrpLaborMarkt[ USERNUMBER ] );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetUser need int parameter" );

	int userNumber = lua_tointeger( L, 2 );
	CNrpUser* user = NULL;
	IF_OBJECT_NOT_NULL_THEN	
	{
		user = _nrpLaborMarkt.GetUser( userNumber );
		//lua_pop( L, argc );
		lua_pushlightuserdata( L, user );
		Luna< CLuaUser >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetUserByName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetUserByName need string parameter" );

	NrpText userName = lua_tostring( L, 2 );

	CNrpUser* user = NULL;
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpUser* ptrUser = _nrpLaborMarkt.GetUser( userName );

		if( ptrUser )
		{
			lua_pushlightuserdata( L, ptrUser );
			Luna< CLuaUser >::constructor( L );
			return 1;	
		}
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::RemoveUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:RemoveUser need IUser* parameter" );

	CNrpUser* user = _GetLuaObject< CNrpUser, CLuaUser >( L, 2 );
	IF_OBJECT_NOT_NULL_THEN	_nrpLaborMarkt.RemoveUser( user );
	return 0;	
}

int CLuaApplication::GetCurrentProfile( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushstring( L, GetParam_<NrpText>( L, PROP, PROFILENAME, "" ) );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetCurrentProfileCompany( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, PROFILECOMPANY, "" ) );
	return 1;
}

int CLuaApplication::CreateProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:CreateProfile need string,string parameters" );

	NrpText userName( lua_tostring( L, 2 ) );
	NrpText companyName( lua_tostring( L, 3 ) );

	IF_OBJECT_NOT_NULL_THEN	_object->CreateProfile( userName, companyName );
	return 0;	
}

int CLuaApplication::ResetData( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:ResetData not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN	_object->ResetData();
	return 0;
}

int CLuaApplication::LoadProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:LoadProfile need string,string parameter" );

	NrpText name( lua_tostring( L, 2 ) );
	NrpText company( lua_tostring( L, 3 ) );

	IF_OBJECT_NOT_NULL_THEN	_object->Load( name, company );

	return 0;	
}

int CLuaApplication::CreateNewFreeUsers( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:UpdateUsers not need parameter" );

	IF_OBJECT_NOT_NULL_THEN	_nrpLaborMarkt.CreateNewFreeUsers();

	return 0;		
}

int CLuaApplication::GetCompanyNumber( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN	
	{
		lua_pushinteger( L, (int)(*_object)[ COMPANIESNUMBER ] );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetUser need int parameter" );

	assert( lua_isnumber( L, 2 ) );
	int cmpNumber = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
    {
        CNrpCompany* ptrCompany = _object->GetCompany( cmpNumber );
        if( ptrCompany )
        {
            lua_pushlightuserdata( L, ptrCompany );
            Luna< CLuaCompany >::constructor( L );
            return 1;
        }
    }

    lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetCompanyByName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetUser need string parameter" );

	NrpText cmpName( lua_tostring( L, 2 ) );
	CNrpCompany* ptrCompany = NULL;

	IF_OBJECT_NOT_NULL_THEN
		ptrCompany = _object->GetCompany( cmpName );

	if( ptrCompany )
	{
		lua_pushlightuserdata( L, ptrCompany );
		Luna< CLuaCompany >::constructor( L );
	}
	else
		lua_pushnil( L );

	return 1;
}

int CLuaApplication::GetGameBoxAddonNumber( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN	
	{
		lua_pushinteger( L, (*_object)[ BOXADDONNUMBER ] );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetGameBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetGameBoxAddon need integer parameter" );

	int addonNumber = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN	
	{
		tech = _object->GetBoxAddon( addonNumber );

		//lua_pop( L, argc );
		if( tech )
		{
			lua_pushlightuserdata( L, tech );
			Luna< CLuaTechnology >::constructor( L );
		}
		else
			lua_pushnil( L );
	}
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
		ret = _object->AddBoxAddon( tech );
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

	IF_OBJECT_NOT_NULL_THEN	_object->AddBoxAddon( tech );

	return 0;	
}

int CLuaApplication::LoadGameTimeFromProfile( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:LoadGameTimeFromProfile need profileName parameter" );

	NrpText profileName( lua_tostring( L, 2 ));

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText pathToFile = (*_object)[ SAVEDIR ];
		pathToFile.append( profileName + "/profile.ini" );
		
		IniFile rv( pathToFile );
		(*_object)[ CURRENTTIME ] = rv.Get( SECTION_PROPERTIES, CURRENTTIME + ":time", NrpTime( 0. ) );
	}

	return 0;	
}

int CLuaApplication::LoadBoxAddonsPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:LoadBoxAddonsPrice not need any parameter" );

	NrpText boxAddonsIni = (NrpText)(*_object)[ SAVEDIR ]+(NrpText)(*_object)[ PROFILENAME ] + "/boxaddons.ini";
	IniFile rv( boxAddonsIni );

	for( int k=0; k < (int)(*_object)[ BOXADDONNUMBER ]; k++ )
	{
		CNrpTechnology& tech = *(*_object).GetBoxAddon( k );
		float price = rv.Get( "prices", tech[ NAME ], (float)tech[ PRICE ] );
		tech[ PRICE ] = price;
	}

	return 0;
}

int CLuaApplication::SaveBoxAddonsPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:SaveBoxAddonsPrice not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpApplication& applic = *_object; 
		NrpText boxAddonsIni = (NrpText)applic[ SAVEDIR ] + (NrpText)applic[ PROFILENAME ]+ "/boxaddons.ini";
		IniFile sv( boxAddonsIni );
		for( int k=0; k < (int)applic[ BOXADDONNUMBER ]; k++ )
		{
			if( CNrpTechnology* tech = applic.GetBoxAddon( k ) )
			{
				sv.Set( "prices", (*tech)[ NAME ], (float)(*tech)[PRICE] );
			}
		}
		sv.Save();
	}
	return 0;
}

int CLuaApplication::GetGamesNumber( lua_State* L )
{
    lua_pushinteger( L, (int)CNrpGameMarket::Instance()[ GAMENUMBER ] );
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
        IF_OBJECT_NOT_NULL_THEN	game = CNrpGameMarket::Instance().GetGame( index );
	}
	else if( lua_isstring( L, 2 ) )
	{
		NrpText name = lua_tostring( L, 2 );
        IF_OBJECT_NOT_NULL_THEN game = CNrpGameMarket::Instance().GetGame( name );
	}

	if( game )
	{
		//lua_pop( L, argc );
		lua_pushlightuserdata( L, game );
		Luna< CLuaGame >::constructor( L );
	}
	else
		lua_pushnil( L );
	return 1;		
}

int CLuaApplication::AddGameToMarket( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:AddGameToMarket need CnrpGame* parameter" );

	CNrpGame* game = _GetLuaObject< CNrpGame, CLuaGame >( L, 2, false );
	assert( game != NULL );

    IF_OBJECT_NOT_NULL_THEN CNrpGameMarket::Instance().StartSale( *game );

	return 0;	
}

int CLuaApplication::LoadScreenshots( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:LoadScreenshots need string, funcName parameter" );

	NrpText imageListName( lua_tostring( L, 2 ) );
    NrpText funcName( lua_tostring( L, 3 ) );

	if( !imageListName.size() )
		return 1;

	_nrpScreenshots.Load( imageListName, funcName );

	return 0;		
}

int CLuaApplication::GetGameTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:GetGameTime not need any parameter" );

	
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpTime time = (*_object)[ CURRENTTIME ];

		lua_pushinteger( L, time.RYear() );
		lua_pushinteger( L, time.RMonth() );
		lua_pushinteger( L, time.RDay() );
		lua_pushinteger( L, time.RHour() );
		lua_pushinteger( L, time.RMinute() );

		return 5;		
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetInvention( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:GetInvention need invention name, company name parameters" );

	NrpText inventionName( lua_tostring( L, 2 ) );
	NrpText companyName( lua_tostring( L, 3 ) );
	CNrpInvention* inv = NULL;

	IF_OBJECT_NOT_NULL_THEN 
	{
		inv = _object->GetInvention( inventionName, companyName );

		//lua_pop( L, argc );
		lua_pushlightuserdata( L, inv );
		Luna< CLuaInvention >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::CreateDirectorySnapshot( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaApplication:CreateDirectorySnapshot need directory, saveFile, itemName, itemTemplate, funcUpdate in parameters" );

	NrpText directory( lua_tostring( L, 2 ) );
	NrpText saveFile( lua_tostring( L, 3 ) );
	NrpText itemTemplate( lua_tostring( L, 4 ) );
	NrpText itemName( lua_tostring( L, 5 ) );
    NrpText funcUpdate( lua_tostring( L, 6 ) );

	OpFileSystem::Remove( saveFile );

	IniFile ini( saveFile );
	OpFileSystem::CreateDirectorySnapshot( directory, itemTemplate, itemName, &ini, funcUpdate );

	ini.Save();
	return 0;
}

int CLuaApplication::GetPda( lua_State* L )
{	
	IF_OBJECT_NOT_NULL_THEN 
	{
		CNrpPda* pda = (*_object)[ PDA ].As<CNrpPda*>();

		//lua_pop( L, lua_gettop( L ) );
		lua_pushlightuserdata( L, pda );
		Luna< CLuaPda >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaApplication::GetPauseBetweenStep( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		int pause = (*_object)[ PAUSEBTWSTEP ];
		lua_pushinteger( L, pause );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaApplication::SetPauseBetweenStep( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		(*_object)[ PAUSEBTWSTEP ] =  lua_tointeger( L, -1 );
	}

	return 0;	
}

int CLuaApplication::SetDevForce( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		(*_object)[ DEV_FORCE ] =  (float)lua_tonumber( L, -1 );
	}

	return 0;	
}

const char* CLuaApplication::ClassName()
{
	return ( CLASS_CLUAPPLICATION );
}

int CLuaApplication::GetPlatformNumber( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
        lua_pushinteger( L, (int)CNrpGameMarket::Instance()[ PLATFORMNUMBER ] );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetPlatform( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:GetPlatform need index parameter" );

	int index = lua_tointeger( L, 2 );
	CNrpPlatform* plt = NULL;
	IF_OBJECT_NOT_NULL_THEN 
	{
        plt = CNrpGameMarket::Instance().GetPlatform( index );

		//lua_pop( L, argc );
		lua_pushlightuserdata( L, plt );
		Luna< CLuaPlatform >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaApplication::LoadPlatform( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:LoadPlatform need PathToFile parameter" );

	NrpText pathTo = lua_tostring( L, 2 );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN 
	{	
		IniFile r( pathTo );

        NrpText plName = r.Get( SECTION_PROPERTIES, INrpConfig::uniqTemplate, NrpText("") );
        if( !CNrpGameMarket::Instance().GetPlatform( plName ) )
		{
			CNrpPlatform* plt = new CNrpPlatform( pathTo );
			plt->Load( pathTo );
			ret = CNrpGameMarket::Instance().AddPlatform( plt );

            assert( ret );
            lua_pushlightuserdata( L, plt );
            Luna< CLuaPlatform >::constructor( L );
            return 1;
		}
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaApplication::LoadLinks( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:LoadLinks need PathToFile, templateName parameter" );

	NrpText pathTo = lua_tostring( L, 2 );
	NrpText tmpName = lua_tostring( L, 3 );

	_nrpLinks.Load( pathTo, tmpName );
	return 0;
}

int CLuaApplication::SinceCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaApplication:SinceCompany need PathToFile parameter" );

	NrpText name = lua_tostring( L, 2 );
    int balance = lua_tointeger( L, 3 );
    int selfPie = lua_tointeger( L, 4 );
    int allPie = lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN 
	{
        CNrpUser* user = _nrpLaborMarkt.CreateRandomUser( CNrpAiUser::ClassName() );
        _nrpLaborMarkt.AddUser( user );
		
        CNrpCompany* cmp = new CNrpCompany( name, user );
        (*cmp)[ BALANCE ] = balance;
        (*cmp)[ SELF_PIE_NUMBER ] = selfPie;
        (*cmp)[ PIE_NUMBER ] = allPie;
        (*cmp)[ STARTDATE ] = _nrpApp[ CURRENTTIME ];

		_object->AddCompany( cmp );
		CNrpBridge::Instance().Update();

		lua_pushlightuserdata( L, cmp );
		Luna< CLuaCompany >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetSoundEngine( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushlightuserdata( L, &CNrpSoundEngine::Instance() );
		Luna< CLuaSoundEngine >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaApplication::GetQuestEngine( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushlightuserdata( L, &CNrpQuestEngine::Instance() );
		Luna< CLuaQuestEngine >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

}//namespace nrp  