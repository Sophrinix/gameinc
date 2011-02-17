#include "StdAfx.h"
#include "LuaFlick.h"

#include "LuaElement.h"
#include "nrpEngine.h"
#include "NrpGuiFlick.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAFLICK( "CLuaFlick" );

BEGIN_LUNA_METHODS(CLuaFlick)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaFlick )
	LUNA_AUTONAME_FUNCTION( CLuaFlick, Clear )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaFlick)
	LUNA_ILUAGUIELEMENT_PROPERTIES( CLuaFlick )
END_LUNA_METHODS

CLuaFlick::CLuaFlick(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUAFLICK, ex)							//конструктор
{}

const char* CLuaFlick::ClassName()
{
	return ( CLASS_LUAFLICK );
}

int CLuaFlick::Clear( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaFlick:Clear not need parameter");
	IF_OBJECT_NOT_NULL_THEN	_object->Clear();
	return 0;		
}

}//namespace nrp
