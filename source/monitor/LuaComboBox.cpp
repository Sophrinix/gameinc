#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaComboBox.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "NrpText.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUACOMBOBOX( "CLuaComboBox" );

Luna< CLuaComboBox >::RegType CLuaComboBox::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaComboBox ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, SetAction ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, AddItem ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, GetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, GetSelectedObject ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, SetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, Clear ),
	{0,0}
};

CLuaComboBox::CLuaComboBox(lua_State *L)	: ILuaGuiElement(L, CLASS_LUACOMBOBOX )							//конструктор
{}

int CLuaComboBox::SetImage( lua_State *L )							//получает имя файла с текстурой, область из которой надо брать кнопку
																	//для текущего состояния
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaComboBox::setImage need 5 parameter");

	/*NrpText texturepath = lua_tostring( L, 6 );
	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	video::ITexture* txs = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturepath.c_str() ); //грузим текстуру в видеокарту
	
	IF_OBJECT_NOT_NULL_THEN	object_->setImage( txs, rectangle );								//размещаем текстуру в кнопке			
	*/

	return 1;
}

int CLuaComboBox::SetAction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaComboBox::SetAction need string parameter");

	return 1;
}

int CLuaComboBox::AddItem( lua_State *L )	//добавляет текст в списко отображения
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaComboBox::AddItem need 2 parameter");

	NrpText text( lua_tostring( L, 2 ) );
	void* object = lua_touserdata( L, 3 );
	
	IF_OBJECT_NOT_NULL_THEN	object_->addItem( text.ToWide(), (u32)object );			

	return 1;
}

int CLuaComboBox::GetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaComboBox::GetSelected not need any parameter");

	int selected = -1;

	IF_OBJECT_NOT_NULL_THEN selected = object_->getSelected();
	lua_pushinteger( L, selected );

	return 1;
}

int CLuaComboBox::SetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaComboBox::SetSelected need 1 parameter");

	int selected = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setSelected( selected );			

	return 1;
}

int CLuaComboBox::Clear( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaComboBox::GetSelected not need any parameter");

	IF_OBJECT_NOT_NULL_THEN object_->clear();

	return 1;
}

int CLuaComboBox::GetSelectedObject( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaComboBox::GetSelected not need any parameter");

	void* ptrData =NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = object_->getSelected();
		if( selected >= 0 )
			ptrData = (void*)object_->getItemData( selected );
	}
	lua_pushlightuserdata( L, ptrData );

	return 1;
}

const char* CLuaComboBox::ClassName()
{
	return ( CLASS_LUACOMBOBOX );
}
}//namespace nrp