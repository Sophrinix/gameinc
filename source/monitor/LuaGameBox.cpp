#include "StdAfx.h"
#include <assert.h>

#include "LuaGameBox.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAGAMEBOX( "CLuaGameBox" );

Luna< CLuaGameBox >::RegType CLuaGameBox::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaGameBox ),
	/*   */
	{0,0}
};

CLuaGameBox::CLuaGameBox(lua_State *L)	: ILuaObject(L, "CLuaGameBox")							//конструктор
{}

const char* CLuaGameBox::ClassName()
{
	return ( CLASS_LUAGAMEBOX );
}

}//namespace nrp