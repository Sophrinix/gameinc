#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaListBox.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "NrpText.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUALISTBOX( "CLuaListBox" );

Luna< CLuaListBox >::RegType CLuaListBox::methods[] =			//реализуемы методы
{
	LUNA_ILUALISTBOX_HEADER( CLuaListBox ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaListBox, AddItem ),
	LUNA_AUTONAME_FUNCTION( CLuaListBox, GetSelectedObject ),
	{0,0}
};

CLuaListBox::CLuaListBox(lua_State *L)	: ILuaListBox(L, CLASS_LUALISTBOX )							//конструктор
{}

int CLuaListBox::AddItem( lua_State *L )	//добавляет текст в списко отображения
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaListBox::AddItem need 2 parameter");

	NrpText text( lua_tostring( L, 2 ) );
	void* object = lua_touserdata( L, 3 );
	
	IF_OBJECT_NOT_NULL_THEN	_object->addItem( text.ToWide(), (u32)object );			

	return 1;
}

int CLuaListBox::GetSelectedObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaListBox::GetSelectedObject not need any parameter");

	void* selObject = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = _object->getSelected();
		if( selected >= 0 )
			selObject = (void*)_object->getIcon( selected );
	}
	lua_pushlightuserdata( L, selObject );

	return 1;
}

const char* CLuaListBox::ClassName()
{
	return ( CLASS_LUALISTBOX );
}
}//namespace nrp