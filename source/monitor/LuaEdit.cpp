#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaEdit.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{

Luna< CLuaEdit >::RegType CLuaEdit::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaEdit ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaEdit, SetOverrideColor ),
	LUNA_AUTONAME_FUNCTION( CLuaEdit, SetPasswordBox ),
	{0,0}
};

CLuaEdit::CLuaEdit(lua_State *L)	: ILuaGuiElement(L, "CLuaEdit")							//конструктор
{}

int CLuaEdit::SetOverrideColor( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaEdit:SetOverrideColor need int parameter");

	int ovColor = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setOverrideColor( ovColor );

	return 1;
}

int CLuaEdit::SetPasswordBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaEdit:SetPasswordBox need int parameter");

	bool isPassBox = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	object_->setPasswordBox( isPassBox );

	return 1;
}

}//namespace nrp