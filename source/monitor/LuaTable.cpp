#include "StdAfx.h"
#include "LuaTable.h"
#include "NrpText.h"
#include "nrpEngine.h"
#include <irrlicht.h>

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUATABLE( "CLuaTable" );

BEGIN_LUNA_METHODS( CLuaTable )
	LUNA_ILUAGUIELEMENT_HEADER( CLuaTable )
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetImage )
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetAction )
	LUNA_AUTONAME_FUNCTION( CLuaTable, AddRow)
	LUNA_AUTONAME_FUNCTION( CLuaTable, AddColumn )
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetColumnWidth )
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetCellText )
	LUNA_AUTONAME_FUNCTION( CLuaTable, GetRowCount )
	LUNA_AUTONAME_FUNCTION( CLuaTable, GetColumnCount )
	LUNA_AUTONAME_FUNCTION( CLuaTable, RemoveColumn )
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetRowHeight )
	LUNA_AUTONAME_FUNCTION( CLuaTable, ClearRows )
	LUNA_AUTONAME_FUNCTION( CLuaTable, GetActiveColumn )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaTable)
	LUNA_ILUAGUIELEMENT_PROPERTIES(CLuaTable)
END_LUNA_PROPERTIES

CLuaTable::CLuaTable(lua_State *L, bool ex) : ILuaGuiElement(L, CLASS_LUATABLE, ex)						//конструктор
{			
}

int CLuaTable::SetImage( lua_State *L )							//получает имя файла с текстурой, область из которой надо брать кнопку
	//для текущего состояния
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaTable::setImage need 5 parameter");

	NrpText texturepath = lua_tostring( L, 6 );

	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN
	{
		video::ITexture* txs = NULL;
		if( texturepath != NULL )
			txs = _nrpEngine.GetVideoDriver()->getTexture( texturepath ); //грузим текстуру в видеокарту
		//object_->setImage( txs, rectangle );								//размещаем текстуру в кнопке			
	}

	return 1;
}

int CLuaTable::SetAction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::SetAction need string parameter");

	NrpText funcName = lua_tostring( L, 2 );
	//dynamic_cast< gui::CNrpButton* >( table_ )->setOnClickAction( funcName.c_str() );

	return 1;
}

int CLuaTable::AddRow( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::AddRow need int parameter");

	int index = lua_tointeger( L, 2 );
	
	IF_OBJECT_NOT_NULL_THEN _object->addRow( index );

	return 1;	
}

int CLuaTable::AddColumn( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaButton::AddColumn need 2 parameter");

	NrpText text( lua_tostring( L, 2 ) );
	int index = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN _object->addColumn( text.ToWide(), index );

	return 1;	
}

int CLuaTable::SetColumnWidth( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaButton::SetColumnWidth need 2 parameter");

	int index = lua_tointeger( L, 2 );
	int width = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN _object->setColumnWidth( index, width );

	return 1;	
}

int CLuaTable::SetCellText( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 8, 8, "Function CLuaButton::SetCellText need 7 parameter");

	int aRow = lua_tointeger( L, 2 );
	int aCol = lua_tointeger( L, 3 );
	NrpText text( lua_tostring( L, 4 ) );

	video::SColor color( lua_tointeger( L, 5 ), lua_tointeger( L, 6 ), lua_tointeger( L, 7 ), lua_tointeger( L, 8 ) );

	IF_OBJECT_NOT_NULL_THEN	_object->setCellText( aRow, aCol, text.ToWide(), video::SColor( color ) );

	return 1;
}

int CLuaTable::GetRowCount( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaButton::GetRowCount not need parameter");

	int rowCount=-9999;

	IF_OBJECT_NOT_NULL_THEN  rowCount = _object->getRowCount();
	lua_pushinteger( L, rowCount );

	return 1;		
}

int CLuaTable::GetColumnCount( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaButton::GetColumnCount not need parameter");

	int rowCount=-9999;

	IF_OBJECT_NOT_NULL_THEN  rowCount = _object->getColumnCount();
	lua_pushinteger( L, rowCount );

	return 1;	
}

int CLuaTable::RemoveColumn( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::RemoveColumn need int parameter");

	int pos=lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN  _object->removeColumn( pos );
	
	return 1;	
}

int CLuaTable::SetRowHeight( lua_State *L )
{
	int argc = lua_gettop(L);luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::SetRowHeigth need 2 parameter");

	int height = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->SetItemHeight( height );
	return 1;	
}

int CLuaTable::ClearRows( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaButton::ClearRows not need parameter");

	IF_OBJECT_NOT_NULL_THEN  _object->clearRows();

	return 1;	
}

const char* CLuaTable::ClassName()
{
	return ( CLASS_LUATABLE );
}

int CLuaTable::GetActiveColumn( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaButton::ClearRows not need parameter");

	IF_OBJECT_NOT_NULL_THEN	lua_pushinteger( L, _object->getActiveColumn() );

	return 1;	
}

}//namespace nrp