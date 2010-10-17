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
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, SetItemHeigth ),
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, SetItemBgColor ),
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, SetFontFromSize ),
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, SetItemTextColor ),
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
	INrpObject* object = (INrpObject*)lua_touserdata( L, 3 );
	
	int ret = -1;
	IF_OBJECT_NOT_NULL_THEN	ret = object_->addItem( StrToWide( text ).c_str(), object, -1 );			

	lua_pushinteger( L, ret );
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

int CLuaComponentListBox::SetItemHeigth( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaComponentListBox::SetSelected need int parameter");

	int height = lua_tointeger( L, 2 );
	assert( height > 0 && height < 200 );

	IF_OBJECT_NOT_NULL_THEN	object_->setItemHeight( height );			

	return 1;
}

int CLuaComponentListBox::SetItemBgColor( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaComponentListBox::GetSelectedObject not need any parameter");

	int index = lua_tointeger( L, 2 );
	video::SColor color( lua_tointeger( L, 3 ), lua_tointeger( L, 4 ), 
		lua_tointeger( L, 5 ), lua_tointeger( L, 6 ) );

	IF_OBJECT_NOT_NULL_THEN object_->setItemOverrideColor( index, gui::EGUI_LBC_BACKGROUND, color );

	return 1;
}

int CLuaComponentListBox::SetFontFromSize( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaComponentListBox::SetFontFromSize need size parameter");

	int size = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		irr::gui::IGUIFont* font = CNrpEngine::Instance().GetGuiEnvironment()->getFont( ("font_" + IntToStr( size )).c_str() );
		object_->setRFont( font );
	}

	return 1;
}

int CLuaComponentListBox::SetItemTextColor( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaComponentListBox::SetTextColor need size parameter");

	int index = lua_tointeger( L, 2 );
	irr::video::SColor color( lua_tointeger( L, 3 ), 
		lua_tointeger( L, 4 ),
		lua_tointeger( L, 5 ),
		lua_tointeger( L, 6 ) );

	IF_OBJECT_NOT_NULL_THEN object_->setItemOverrideColor( index, gui::EGUI_LBC_TEXT, color );

	return 1;
}
}//namespace nrp