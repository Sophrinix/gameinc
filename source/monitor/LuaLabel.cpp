#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaLabel.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{

Luna< CLuaLabel >::RegType CLuaLabel::methods[] =			//���������� ������
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaLabel ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaLabel, SetOverrideColor ),
	{0,0}
};

CLuaLabel::CLuaLabel(lua_State *L)	: ILuaGuiElement(L, "CLuaLabel")							//�����������
{}

int CLuaLabel::SetOverrideColor( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaLabel::SetOverrideColor need 4 int parameter");

	int ovColor = (lua_tointeger( L, 2 ) << 24);
	ovColor += (lua_tointeger( L, 3 ) << 16);
	ovColor += (lua_tointeger( L, 4 ) << 8);
	ovColor += lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN	object_->setOverrideColor( ovColor );

	return 1;
}
}//namespace nrp