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

Luna< CLuaPictureFlow >::RegType CLuaPictureFlow::methods[] =			//���������� ������
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaPictureFlow ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, AddItem ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, GetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, Clear ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, GetSelectedObject ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, GetSelectedItem ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetPictureRect ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetDrawBorder ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetItemTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetItemBlend ),
	{0,0}
};

CLuaPictureFlow::CLuaPictureFlow(lua_State *L)	: ILuaGuiElement(L, CLASS_LUAPICTUREFLOW )							//�����������
{}

int CLuaPictureFlow::AddItem( lua_State *L )	//��������� ����� � ������ �����������
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaPictureFlow::AddItem need 2 parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText texturePath( lua_tostring( L, 2 ) );
		video::ITexture* texture = _nrpEngine.GetVideoDriver()->getTexture( texturePath );

		NrpText text( lua_tostring( L, 3 ) ); 
		void* object = _GetLuaObject< void, ILuaObject >( L, 4, true );
	
		int ret = _object->addItem( texture, text.ToWide(), object );			
		lua_pushinteger( L, ret );
	}

	return 1;
}

int CLuaPictureFlow::GetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPictureFlow::GetSelected not need any parameter");

	int selected = -1;

	IF_OBJECT_NOT_NULL_THEN selected = _object->getSelected();
	lua_pushinteger( L, selected );

	return 1;
}

int CLuaPictureFlow::SetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPictureFlow::SetSelected need int parameter");

	int selected = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	_object->setSelected( selected );			

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
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPictureFlow::GetSelectedObject not need any parameter");

	void* selObject = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = _object->getSelected();
		if( selected >= 0 )
			selObject = (void*)_object->getObject( selected );
	}
	lua_pushlightuserdata( L, selObject );

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

int CLuaPictureFlow::GetSelectedItem( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPictureFlow::GetSelectedObject not need any parameter");

	NrpText text( "" );

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = _object->getSelected();
		if( selected >= 0 )
			text = _object->getListItem( selected );
	}

	lua_pushstring( L, text );
	return 1;
}

int CLuaPictureFlow::SetDrawBorder( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPictureFlow::SetDrawBorder need boolean parameter");

	bool drawBorder = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	_object->setDrawBackground( drawBorder );			

	return 1;
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
}//namespace nrp