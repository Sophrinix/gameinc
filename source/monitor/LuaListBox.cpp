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
	LUNA_ILUAGUIELEMENT_HEADER( CLuaListBox ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaListBox, AddItem ),
	LUNA_AUTONAME_FUNCTION( CLuaListBox, GetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaListBox, SetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaListBox, Clear ),
	LUNA_AUTONAME_FUNCTION( CLuaListBox, GetSelectedObject ),
	LUNA_AUTONAME_FUNCTION( CLuaListBox, SetItemBgColor ),
	LUNA_AUTONAME_FUNCTION( CLuaListBox, SetFontFromSize ),
	LUNA_AUTONAME_FUNCTION( CLuaListBox, SetItemTextColor ),
	{0,0}
};

CLuaListBox::CLuaListBox(lua_State *L)	: ILuaGuiElement(L, "CLuaListBox")							//конструктор
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

int CLuaListBox::GetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaListBox::GetSelected not need any parameter");

	int selected = -1;

	IF_OBJECT_NOT_NULL_THEN selected = object_->getSelected();
	lua_pushinteger( L, selected );

	return 1;
}

int CLuaListBox::SetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaListBox::SetSelected need int parameter");

	int selected = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setSelected( selected );			

	return 1;
}

int CLuaListBox::Clear( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaListBox::Clear not need any parameter");

	IF_OBJECT_NOT_NULL_THEN	object_->clear();			

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

int CLuaListBox::SetItemBgColor( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaListBox::GetSelectedObject not need any parameter");

	int index = lua_tointeger( L, 2 );
	video::SColor color( lua_tointeger( L, 3 ), lua_tointeger( L, 4 ), 
						 lua_tointeger( L, 5 ), lua_tointeger( L, 6 ) );

	IF_OBJECT_NOT_NULL_THEN object_->setItemOverrideColor( index, gui::EGUI_LBC_BACKGROUND, color );

	return 1;
}

int CLuaListBox::SetFontFromSize( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaListBox::SetFontFromSize need size parameter");

	int size = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		irr::gui::IGUIFont* font = CNrpEngine::Instance().GetGuiEnvironment()->getFont( ("font_" + IntToStr( size )).c_str() );
		object_->setRFont( font );
	}

	return 1;
}

int CLuaListBox::SetItemTextColor( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaListBox::SetTextColor need size parameter");

	int index = lua_tointeger( L, 2 );
	irr::video::SColor color( lua_tointeger( L, 3 ), 
		lua_tointeger( L, 4 ),
		lua_tointeger( L, 5 ),
		lua_tointeger( L, 6 ) );

	IF_OBJECT_NOT_NULL_THEN object_->setItemOverrideColor( index, gui::EGUI_LBC_TEXT, color );

	return 1;
}
}//namespace nrp