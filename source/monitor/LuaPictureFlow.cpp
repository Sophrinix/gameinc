#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaPictureFlow.h"
#include "nrpEngine.h"
#include "Nrp2DPictureFlow.h"
#include "nrpGUIEnvironment.h"
#include "NrpText.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAPICTUREFLOW( "CLuaPictureFlow" );

BEGIN_LUNA_METHODS(CLuaPictureFlow)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaPictureFlow )
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, AddItem )
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, Clear )
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetPictureRect )
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetItemTexture )
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetItemBlend )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaPictureFlow)
	LUNA_ILUAGUIELEMENT_PROPERTIES( CLuaPictureFlow )
	LUNA_AUTONAME_PROPERTY( CLuaPictureFlow, "selectedIndex", GetSelected, SetSelected )
	LUNA_AUTONAME_PROPERTY( CLuaPictureFlow, "selectedObject", GetSelectedObject, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPictureFlow, "selectedText", GetSelectedText, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPictureFlow, "count", GetCount, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPictureFlow, "drawBody", PureFunction, SetDrawBorder )
	LUNA_AUTONAME_PROPERTY( CLuaPictureFlow, "onSelect", PureFunction, SetOnNewSelect )
END_LUNA_PROPERTIES

CLuaPictureFlow::CLuaPictureFlow(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUAPICTUREFLOW, ex )							//конструктор
{}

int CLuaPictureFlow::AddItem( lua_State *L )	//добавляет текст в списко отображения
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaPictureFlow::AddItem need 2 parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText texturePath( lua_tostring( L, 2 ) );

		NrpText text( lua_tostring( L, 3 ) ); 
		void* object = _GetLuaObject< void, ILuaObject >( L, 4, true );
	
		int ret = _object->addItem( texturePath.ToWide(), text.ToWide(), object );			
		lua_pushinteger( L, ret );
	}

	return 1;
}

int CLuaPictureFlow::GetSelected( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		lua_pushinteger( L, _object->getSelected() );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaPictureFlow::SetSelected( lua_State *L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN _object->setSelected( lua_tointeger( L, -1 ) );			

	return 0;
}

int CLuaPictureFlow::GetCount( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		lua_pushinteger( L, _object->getItemCount() );			
		return 1;
	}
	
	lua_pushnil( L );
	return 1;
}

int CLuaPictureFlow::Clear( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPictureFlow::Clear not need any parameter");

	IF_OBJECT_NOT_NULL_THEN	_object->clear();			

	return 1;
}

int CLuaPictureFlow::GetSelectedObject( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		void* selObject = NULL;
		int selected = _object->getSelected();
		if( selected >= 0 )
			selObject = (void*)_object->getObject( selected );
		lua_pushlightuserdata( L, selObject );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaPictureFlow::SetPictureRect( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaPictureFlow::SetPictureRect need rectangle parameter");

	core::recti rectangle( lua_tointeger( L, 2 ), lua_tointeger( L, 3 ), lua_tointeger( L, 4 ), lua_tointeger( L, 5) );

	IF_OBJECT_NOT_NULL_THEN	_object->setPictureRect( rectangle );

	return 1;
}

int CLuaPictureFlow::GetSelectedText( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText text( "" );
		int selected = _object->getSelected();
		if( selected >= 0 )
			text = _object->getListItem( selected );
		lua_pushstring( L, text );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaPictureFlow::SetDrawBorder( lua_State* L )
{
	assert( lua_isboolean( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		bool drawBorder = lua_toboolean( L, -1 ) > 0;
		_object->setDrawBackground( drawBorder );			
	}

	return 0;
}

const char* CLuaPictureFlow::ClassName()
{
	return ( CLASS_LUAPICTUREFLOW );
}

int CLuaPictureFlow::SetItemTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPictureFlow::SetItemTexture need index, pathToTexure parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		int index = lua_tointeger( L, 2 );
		NrpText texturePath = lua_tostring( L, 3 );
		video::ITexture* texture = _nrpEngine.GetVideoDriver()->getTexture( texturePath );

		_object->setItemTexture( index, texture );			
	}

	return 1;
}

int CLuaPictureFlow::SetItemBlend( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPictureFlow::SetItemBlend need int, int parameter");

	int index = lua_tointeger( L, 2 );
	int blend = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->setItemBlend( index, blend );			

	return 1;
}

int CLuaPictureFlow::SetOnNewSelect( lua_State* L )
{
	assert( lua_isfunction( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		_object->Bind( GUIELEMENT_LBXITEM_SELECTED, _GetRef( L, -1 ) );			
	}

	return 0;
}

}//namespace nrp