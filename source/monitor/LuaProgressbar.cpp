#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaProgressBar.h"
#include "NrpProgressBar.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{

Luna< CLuaProgressBar >::RegType CLuaProgressBar::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaProgressBar ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaProgressBar, SetPosition ),
	{0,0}
};

CLuaProgressBar::CLuaProgressBar(lua_State *L)	: ILuaGuiElement(L, "CLuaProgressBar")							//конструктор
{}

int CLuaProgressBar::SetPosition( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLabel:SetPosition need int parameter");

	int position = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setPosition( position );

	return 1;
}
}//namespace nrp