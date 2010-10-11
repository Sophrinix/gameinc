#include "StdAfx.h"

#include "LuaBrowser.h"
#include "HTMLEngine.h"
#include "NrpBrowserWindow.h"
#include "NrpApplication.h"

#include <assert.h>
#include <irrlicht.h>

using namespace irr::core;

namespace nrp
{

Luna< CLuaBrowser >::RegType CLuaBrowser::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaBrowser ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Show ),
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Hide ),
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Navigate ),
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, GetWindow ),
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Move ),
	{0,0}
};

CLuaBrowser::CLuaBrowser(lua_State *L)	: ILuaProject(L, CLASS_LUABROWSER )	//конструктор
{}

int CLuaBrowser::Show( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaBrowser::Show not need any parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		irr::gui::CNrpBrowserWindow& wnd = object_->GetBrowserWindow( dimension2du( 512, 512 ) );
		wnd.setVisible( true );
	}

	return 1;	
}

int CLuaBrowser::Navigate( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaBrowser::Navigate need string parameter");

	const char* pathto = lua_tostring( L, 2 );
	assert( pathto != NULL );
	if( pathto == NULL )
		return 1;
	
	IF_OBJECT_NOT_NULL_THEN 
	{
		std::string advpath = pathto;
		//если путь относительный надо его привести к абсолютному... иначи геко споткнется
		//и не сможет обработать страницу
		if( advpath.find( ':' ) == -1 )
			advpath = CNrpApplication::Instance().GetValue<std::string>( WORKDIR ) + advpath;
		object_->Navigate( "file://" + advpath );
	}

	return 1;	
}

int CLuaBrowser::Hide( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaBrowser::Show not need any parameter");

	IF_OBJECT_NOT_NULL_THEN 
	{
		irr::gui::CNrpBrowserWindow& wnd = object_->GetBrowserWindow();
		wnd.setVisible( false );
	}

	return 1;	
}

int CLuaBrowser::Move( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaBrowser::Move need x, y parameter");
	
	position2di pos( lua_tointeger( L, 2 ), lua_tointeger( L, 3 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		object_->GetBrowserWindow().setRelativePosition( pos );
	}

	return 1;
}

int CLuaBrowser::GetWindow( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaBrowser::GetWindow not need any parameter");

	irr::gui::CNrpBrowserWindow* wnd = NULL;
	IF_OBJECT_NOT_NULL_THEN wnd = &(object_->GetBrowserWindow());

	lua_pushlightuserdata( L, wnd );
	return 1;	
}
}//namespace nrp