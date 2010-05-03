#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaComponentListBox.h"
#include "NrpComponentListbox.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "StrConversation.h"

using namespace irr;

namespace nrp
{

Luna< CLuaComponentListBox >::RegType CLuaComponentListBox::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaComponentListBox ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, AddItem ),
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, GetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, SetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, Clear ),
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, GetSelectedObject ),
	{0,0}
};

CLuaComponentListBox::CLuaComponentListBox(lua_State *L) : ILuaGuiElement( L, "CLuaComponentListBox" )							//конструктор
{}

int CLuaComponentListBox::AddItem( lua_State *L )	//добавляет текст в списко отображения
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaComponentListBox::AddItem need 2 parameter");

	const char* text = lua_tostring( L, 2 );
	assert( text != NULL );
	void* object = lua_touserdata( L, 3 );
	
	IF_OBJECT_NOT_NULL_THEN	object_->addItem( StrToWide( text ).c_str(), object, -1 );			

	return 1;
}

int CLuaComponentListBox::GetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaComponentListBox::GetSelected not need any parameter");

	int selected = -1;

	IF_OBJECT_NOT_NULL_THEN selected = object_->getSelected();
	lua_pushinteger( L, selected );

	return 1;
}

int CLuaComponentListBox::SetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaComponentListBox::SetSelected need int parameter");

	int selected = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setSelected( selected );			

	return 1;
}

int CLuaComponentListBox::Clear( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaComponentListBox::Clear not need any parameter");

	IF_OBJECT_NOT_NULL_THEN	object_->clear();			

	return 1;
}

int CLuaComponentListBox::GetSelectedObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaComponentListBox::GetSelectedObject not need any parameter");

	void* selObject = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = object_->getSelected();
		if( selected >= 0 )
			selObject = object_->getSelectedObject();
	}
	lua_pushlightuserdata( L, selObject );

	return 1;
}
}//namespace nrp