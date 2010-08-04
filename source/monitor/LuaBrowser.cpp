#include "StdAfx.h"

#include "LuaBrowser.h"
#include "HTMLEngine.h"
#include "NrpBrowserWindow.h"

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
	{0,0}
};

CLuaBrowser::CLuaBrowser(lua_State *L)	: ILuaProject(L, CLASS_LUABROWSER )	//конструктор
{}

int CLuaBrowser::Show( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaBrowser::Show not need any parameter");

	IF_OBJECT_NOT_NULL_THEN object_->GetBrowserWindow( dimension2du( 512, 512 ) );
	object_->Navigate( "www.google.ru" );

	return 1;	
}
}//namespace nrp