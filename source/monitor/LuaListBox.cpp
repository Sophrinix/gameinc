#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaListBox.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "StrConversation.h"

using namespace irr;

namespace nrp
{

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

	const char* text = lua_tostring( L, 2 );
	assert( text != NULL );
	void* object = lua_touserdata( L, 3 );
	
	IF_OBJECT_NOT_NULL_THEN	object_->addItem( StrToWide( text ).c_str(), (u32)object );			

	return 1;
}

int CLuaListBox::GetSelectedObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaListBox::GetSelectedObject not need any parameter");

	void* selObject = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = object_->getSelected();
		if( selected >= 0 )
			selObject = (void*)object_->getIcon( selected );
	}
	lua_pushlightuserdata( L, selObject );

	return 1;
}

}//namespace nrp