#include "StdAfx.h"
#include <assert.h>

#include "LuaGame.h"
#include "NrpGame.h"
#include "NrpGameBox.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"
#include "NrpGameImageList.h"

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
			isMyBox = box->IsMyBoxAddon( name );

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
			box->RemoveMyBoxAddon( name );

	}
	return 1;	
}

int CLuaGame::AddBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:AddBoxAddon need CNrpTechnology parameter" );

	PNrpTechnology tech = (PNrpTechnology)lua_touserdata( L, 2 );
	assert( tech != NULL );
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		assert( box != NULL );
		if( box != NULL )
			box->AddBoxAddon( tech );

	}
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

	lua_pushlightuserdata( L, tech );
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
		CNrpGameImageList* imageList = object_->GetValue<CNrpGameImageList*>( GAMEIMAGELIST );
		if( imageList )
		{
			int maxImage = imageList->GetValue<int>( nameParam );
			if( maxImage > 0)
			{
				std::vector< std::string >& pvm = (nameParam == IMAGESBOXNUMBER) 
													? imageList->imagesBoxPath 
													: imageList->imagesPath;
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
		CNrpGameImageList* imageList = object_->GetValue<CNrpGameImageList*>( GAMEIMAGELIST );
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
}//namespace nrp