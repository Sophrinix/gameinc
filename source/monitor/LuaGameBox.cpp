#include "StdAfx.h"
#include <assert.h>

#include "LuaGameBox.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAGAMEBOX( "CLuaGameBox" );

BEGIN_LUNA_METHODS(CLuaGameBox)
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaGameBox)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaGameBox )
END_LUNA_METHODS

CLuaGameBox::CLuaGameBox(lua_State *L, bool ex)	: ILuaObject(L, CLASS_LUAGAMEBOX, ex)							//конструктор
{}

const char* CLuaGameBox::ClassName()
{
	return ( CLASS_LUAGAMEBOX );
}

}//namespace nrp