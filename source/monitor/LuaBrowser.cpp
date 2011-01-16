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
CLASS_NAME CLASS_LUABROWSER( "CLuaBrowser" );

BEGIN_LUNA_METHODS(CLuaBrowser)
	LUNA_ILUAOBJECT_HEADER( CLuaBrowser )
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Show )
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Hide )
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Navigate )
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, GetWindow )
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Move )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaBrowser)
END_LUNA_PROPERTIES

CLuaBrowser::CLuaBrowser(lua_State *L, bool ex)	: ILuaProject(L, CLASS_LUABROWSER, ex )	//конструктор
{}

int CLuaBrowser::Show( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaBrowser::Show not need any parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		irr::gui::CNrpBrowserWindow& wnd = _object->GetBrowserWindow( dimension2du( 512, 512 ) );
		wnd.setVisible( true );
	}

	return 1;	
}

int CLuaBrowser::Navigate( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaBrowser::Navigate need string parameter");

	NrpText pathto = lua_tostring( L, 2 );
	if( !pathto.size() )
		return 1;
	
	IF_OBJECT_NOT_NULL_THEN 
	{
		NrpText advpath = pathto;
		//если путь относительный надо его привести к абсолютному... иначи геко споткнется
		//и не сможет обработать страницу
		if( advpath.findFirst( L':' ) == -1 )
			advpath = (NrpText)_nrpApp[ WORKDIR ] + advpath;

		_object->Navigate( NrpText("file://") + advpath );
	}

	return 1;	
}

int CLuaBrowser::Hide( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaBrowser::Show not need any parameter");

	IF_OBJECT_NOT_NULL_THEN 
	{
		irr::gui::CNrpBrowserWindow& wnd = _object->GetBrowserWindow();
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
		_object->GetBrowserWindow().setRelativePosition( pos );
	}

	return 1;
}

int CLuaBrowser::GetWindow( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaBrowser::GetWindow not need any parameter");

	irr::gui::CNrpBrowserWindow* wnd = NULL;
	IF_OBJECT_NOT_NULL_THEN wnd = &(_object->GetBrowserWindow());

	lua_pushlightuserdata( L, wnd );
	return 1;	
}

const char* CLuaBrowser::ClassName()
{
	return ( CLASS_LUABROWSER );
}

}//namespace nrp