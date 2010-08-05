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
	LUNA_AUTONAME_FUNCTION( CLuaBrowser, Navigate ),
	{0,0}
};

CLuaBrowser::CLuaBrowser(lua_State *L)	: ILuaProject(L, CLASS_LUABROWSER )	//конструктор
{}

int CLuaBrowser::Show( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaBrowser::Show not need any parameter");

	IF_OBJECT_NOT_NULL_THEN object_->GetBrowserWindow( dimension2du( 512, 512 ) );

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
			advpath = CNrpApplication::Instance().GetValue<std::string>( FULLPATH ) + advpath;
		object_->Navigate( "file://" + advpath );
	}

	return 1;	
}
}//namespace nrp