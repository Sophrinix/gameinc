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

BEGIN_LUNA_METHODS(CLuaComboBox)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaComboBox )
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, SetImage )
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, SetAction )
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, AddItem )
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, GetItem )
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, Clear )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaComboBox)
	LUNA_ILUAGUIELEMENT_PROPERTIES( CLuaComboBox )
	LUNA_AUTONAME_PROPERTY( CLuaComboBox, "itemCount", GetItemCount, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaComboBox, "selectedText", GetSelectedText, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaComboBox, "selectedObject", GetSelectedObject, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaComboBox, "itemIndex", GetSelected, SetSelected )
	//LUNA_AUTONAME_PROPERTY
END_LUNA_PROPERTIES

CLuaComboBox::CLuaComboBox(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUACOMBOBOX, ex )							//конструктор
{}

int CLuaComboBox::GetItemCount( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, _object->getItemCount() );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaComboBox::GetSelectedText( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaComboBox::GetSelectedText need index parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = _object->getSelected();
		if( selected >= 0 )
		{
			NrpText text( _object->getItem( selected ) );
			lua_pushstring( L, text );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;
}

int CLuaComboBox::GetItem( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaComboBox::GetItem need index parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = _object->getSelected();
		if( selected >= 0 )
		{
			void* ptrData = (void*)_object->getItemData( selected );
			lua_pushlightuserdata( L, ptrData );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;
}

int CLuaComboBox::SetImage( lua_State *L )							//получает имя файла с текстурой
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaComboBox::setImage need 5 parameter");

	/*NrpText texturepath = lua_tostring( L, 6 );
	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	video::ITexture* txs = _nrpEngine.GetVideoDriver()->getTexture( texturepath.c_str() ); //грузим текстуру в видеокарту
	
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
	void* object = _GetLuaObject< void, ILuaObject >( L, 3, true );
	assert( object != NULL );
	
	IF_OBJECT_NOT_NULL_THEN	_object->addItem( text.ToWide(), (u32)object );			

	return 1;
}

int CLuaComboBox::GetSelected( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		int selected = _object->getSelected();
		lua_pushinteger( L, selected );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaComboBox::SetSelected( lua_State *L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	_object->setSelected( lua_tointeger( L, -1 ) );			

	return 0;
}

int CLuaComboBox::Clear( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaComboBox::GetSelected not need any parameter");

	IF_OBJECT_NOT_NULL_THEN _object->clear();

	return 1;
}

int CLuaComboBox::GetSelectedObject( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		void* ptrData = NULL;
		int selected = _object->getSelected();
		if( selected >= 0 )
			ptrData = (void*)_object->getItemData( selected );
		lua_pushlightuserdata( L, ptrData );
		return 1;
	}
	
	lua_pushnil( L );
	return 1;
}

const char* CLuaComboBox::ClassName()
{
	return ( CLASS_LUACOMBOBOX );
}
}//namespace nrp