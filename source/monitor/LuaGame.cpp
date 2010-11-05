#include "StdAfx.h"
#include <assert.h>

#include "LuaGame.h"
#include "NrpGame.h"
#include "NrpGameBox.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"
#include "NrpScreenshot.h"
#include "LuaTechnology.h"
#include "LuaCompany.h"
#include "NrpHistory.h"

using namespace irr;

namespace nrp
{

Luna< CLuaGame >::RegType CLuaGame::methods[] =			//реализуемы методы
{
	LUNA_ILUABASEPROJECT_HEADER( CLuaGame ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaGame, HaveBox ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetName ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, IsMyBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, RemoveBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, AddBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxAddonsNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, CreateBox ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, RemoveBox ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, IsSaling ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxImage ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxImageNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetScreenshot ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetScreenshotNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, SetViewImage ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetViewImage ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetLastMonthSales ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetCurrentMonthSales ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetAllTimeSales ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, SetPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetCompany ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetDescriptionLink ),
	{0,0}
};

CLuaGame::CLuaGame(lua_State *L) : ILuaBaseProject(L, CLASS_LUAGAME )							//конструктор
{}

int CLuaGame::HaveBox( lua_State* L )
{
	lua_pushboolean( L, GetParam_<PNrpGameBox>( L, "HaveBox", GBOX, NULL ) != NULL );
	return 1;	
}

int CLuaGame::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetName", NAME, "undeclared param" ).c_str() );
	return 1;	
}

int CLuaGame::IsMyBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:IsMyBoxAddon need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	bool isMyBox = false;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		if( box != NULL )
			isMyBox = box->IsMyAddon( name );

	}
	lua_pushboolean( L, isMyBox );
	return 1;	
}

int CLuaGame::RemoveBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:RemoveBoxAddon need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		if( box != NULL )
			box->RemoveAddon( name );
	}
	return 1;	
}

int CLuaGame::AddBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:AddBoxAddon need CNrpTechnology parameter" );

	CNrpBoxAddon* addon = (CNrpBoxAddon*)lua_touserdata( L, 2 );
	assert( addon != NULL );

	bool ret=false;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		assert( box != NULL );
		if( box != NULL )
			ret = box->AddAddon( addon );
	}

	lua_pushboolean( L, ret );
	return 1;	
}

int CLuaGame::GetBoxAddonsNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:GetGameBoxAddonsNumber not need any parameter" );

	int addonNumber = 0;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		if( box != NULL)
			addonNumber = box->GetValue<int>( NUMBERADDON );
	}

	lua_pushinteger( L, addonNumber );
	return 1;		
}

int CLuaGame::CreateBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:CreateBox need int parameter" );

	int boxLevel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = new CNrpGameBox( object_ );
		box->SetValue<int>( LEVEL, boxLevel );
		object_->SetValue<PNrpGameBox>( GBOX, box );
	}
	return 1;		
}

int CLuaGame::RemoveBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:RemoveBox not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		object_->SetValue<PNrpGameBox>( GBOX, NULL );
		delete box;
	}

	return 1;			
}

int CLuaGame::GetBoxLevel( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:GetBoxLevel not need any parameter" );

	int boxLevel = 0;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		if( box != NULL )
			boxLevel = box->GetValue<int>( LEVEL );
	}

	lua_pushinteger( L, boxLevel );
	return 1;			
}

int CLuaGame::GetBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:GetBoxAddon need int parameter" );

	int index = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		if( box )
			tech = box->GetAddon( index );
	}

	lua_pop( L, argc );
	lua_pushlightuserdata( L, tech );
	Luna< CLuaTechnology >::constructor( L );

	return 1;		
}

int CLuaGame::IsSaling( lua_State* L )
{
	lua_pushboolean( L, GetParam_<bool>( L, "ISSALING", GAMEISSALING, true ) );
	return 1;		
}

int CLuaGame::GetImagePath_( lua_State* L, std::string funcName, std::string nameParam )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, ("Function CLuaGame:" + funcName + " need index parameter").c_str() );

	int index = lua_tointeger( L, 2 );
	std::string pathTexture = "";
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpScreenshot* imageList = object_->GetValue<CNrpScreenshot*>( GAMEIMAGELIST );
		if( imageList )
		{
			int maxImage = imageList->GetValue<int>( nameParam );
			if( maxImage > 0)
			{
				const std::vector< std::string >& pvm = (nameParam == IMAGESBOXNUMBER) 
													? imageList->GetBoxImages() 
													: imageList->GetImages();
				pathTexture = pvm[ index < 0 ? (rand() % maxImage) : index ];
			}
		}
	}

	lua_pushstring( L, pathTexture.c_str() );
	return 1;		
}

int CLuaGame::GetBoxImage( lua_State* L )
{
	return GetImagePath_( L, "GetBoxImage", IMAGESBOXNUMBER );
}

template< class R > R CLuaGame::GetImageLisParam_( lua_State* L, std::string funcName, std::string name, R defValue )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, ("Function CLuaGame:" + funcName + " not need any parameters").c_str() );

	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpScreenshot* imageList = object_->GetValue<CNrpScreenshot*>( GAMEIMAGELIST );
		if( imageList )
			defValue = imageList->GetValue<int>( name );
	}

	return defValue;
}

int CLuaGame::GetBoxImageNumber( lua_State* L )
{
	lua_pushinteger( L, GetImageLisParam_<int>( L, "GetBoxImageNumber", IMAGESBOXNUMBER, 0 ) );
	return 1;		
}

int CLuaGame::GetScreenshotNumber( lua_State* L )
{
	lua_pushinteger( L, GetImageLisParam_<int>( L, "GetScreenshotNumber", IMAGESNUMBER, 0 ) ); 
	return 1;
}

int CLuaGame::GetScreenshot( lua_State* L )
{
	return GetImagePath_( L, "GetScreenShot", IMAGESNUMBER );
}

int CLuaGame::SetViewImage( lua_State* L )
{
	return SetParam_( L, "SetViewImage", VIEWIMAGE );
}

int CLuaGame::GetViewImage( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetViewImage", VIEWIMAGE, "undeclared param" ).c_str() );
	return 1;	
}

int CLuaGame::GetCurrentMonthSales( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:GetLastMonthSales not need any parameters" );

	int lastMonthSales = 0;
	IF_OBJECT_NOT_NULL_THEN
		if( object_->GetHistory()->GetValue<int>( HISTORY_SIZE ) > 0 )
		{
			//CNrpGame::SALE_HISTORY_MAP::const_iterator pIter = object_->GetSalesHistory().end();
			//pIter--;
			//lastMonthSales = pIter->second->numberSale;	
		}

		lua_pushinteger( L, lastMonthSales );
		return 1;
}

int CLuaGame::GetLastMonthSales( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:GetLastMonthSales not need any parameters" );

	int lastMonthSales = 0;
	IF_OBJECT_NOT_NULL_THEN
		if( object_->GetHistory()->GetValue<int>( HISTORY_SIZE ) > 0 )
		{
			//CNrpGame::SALE_HISTORY_MAP::const_iterator pIter = object_->GetSalesHistory().end();
			//pIter--; pIter--;
			//lastMonthSales = pIter->second->numberSale;	
		}

	lua_pushinteger( L, lastMonthSales );
	return 1;
}

int CLuaGame::GetAllTimeSales( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetAllTimeSales", COPYSELL, 0 ) );
	return 1;		
}

int CLuaGame::GetPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:GetPrice not need any parameter" );

	int price = 0;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		assert( box != NULL );
		if( box != NULL )
			price = box->GetValue<int>( PRICE );
	}

	lua_pushinteger( L, price );
	return 1;		
}

int CLuaGame::SetPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:SetPrice need int parameter" );

	int newPrice = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		assert( box != NULL );
		if( box )
			box->SetValue<int>( PRICE, newPrice );
	}
	return 1;		
}

int CLuaGame::GetCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:GetCompany not need any parameter" );

	CNrpCompany* cmp = NULL;
	IF_OBJECT_NOT_NULL_THEN cmp = object_->GetValue<CNrpCompany*>( PARENTCOMPANY );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, cmp );
	Luna< CLuaCompany >::constructor( L );
	return 1;		
}

int CLuaGame::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:Create need string parameter" );

	const char* fileName = lua_tostring( L, 2 );
	assert( fileName != NULL );

	if( fileName != NULL )
		object_ = new CNrpGame( fileName  );

	return 1;		
}

int CLuaGame::GetDescriptionLink( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetDescriptionLink", DESCRIPTIONPATH, "" ).c_str() );
	return 1;	
}
}//namespace nrp