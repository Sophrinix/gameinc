#include "StdAfx.h"
#include <IGUIComboBox.h>
#include <irrlicht.h>

#include "LuaComboBox.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "StrConversation.h"

using namespace irr;

namespace nrp
{

Luna< CLuaComboBox >::RegType CLuaComboBox::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaComboBox ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, SetAction ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, AddItem ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, GetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaComboBox, SetSelected ),
	{0,0}
};

CLuaComboBox::CLuaComboBox(lua_State *L)	: ILuaGuiElement(L, "CLuaComboBox")							//конструктор
{}

int CLuaComboBox::SetImage( lua_State *L )							//получает имя файла с текстурой, область из которой надо брать кнопку
																	//для текущего состояния
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaComboBox::setImage need 5 parameter");

	/*std::string texturepath = lua_tostring( L, 6 );
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

	/*std::string funcName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	dynamic_cast< gui::CNrpComboBox* >( object_ )->setOnClickAction( funcName.c_str() );

#ifdef _DEBUG
	char text[ MAX_PATH ];
	sprintf_s( text, MAX_PATH, "Object: %d  FuncName:%s\n", object_, funcName.c_str() );
	OutputDebugString( text );
#endif
	*/
	return 1;
}

int CLuaComboBox::AddItem( lua_State *L )	//добавляет текст в списко отображения
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaComboBox::AddItem need 2 parameter");

	std::string text = lua_tostring( L, 2 );
	void* object = lua_touserdata( L, 3 );
	
	IF_OBJECT_NOT_NULL_THEN	object_->addItem( StrToWide( text ).c_str(), (u32)object );			

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
}//namespace nrp