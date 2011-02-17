#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaLinkBox.h"
#include "nrpGuiLinkBox.h"
#include "nrpEngine.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LINKBOX( "CLuaLinkBox" );

BEGIN_LUNA_METHODS(CLuaLinkBox)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaLinkBox )
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, AddLuaFunction )
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, RemoveLuaFunction )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaLinkBox)
	LUNA_ILUAGUIELEMENT_PROPERTIES( CLuaLinkBox )
	LUNA_AUTONAME_PROPERTY( CLuaLinkBox, "type", GetModuleType, SetModuleType )
	LUNA_AUTONAME_PROPERTY( CLuaLinkBox, "data", GetData, SetData )
	LUNA_AUTONAME_PROPERTY( CLuaLinkBox, "haveData", HaveData, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaLinkBox, "draggable", IsDraggable, SetDraggable )
	LUNA_AUTONAME_PROPERTY( CLuaLinkBox, "texture", GetTexture, SetTexture )
	LUNA_AUTONAME_PROPERTY( CLuaLinkBox, "defaultTexture", PureFunction, SetDefaultTexture )
	LUNA_AUTONAME_PROPERTY( CLuaLinkBox, "setFunction", PureFunction, SetFunction )
	LUNA_AUTONAME_PROPERTY( CLuaLinkBox, "unsetFunction", PureFunction, UnsetFunction )
END_LUNA_PROPERTIES

CLuaLinkBox::CLuaLinkBox(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LINKBOX, ex )							//конструктор
{}

int CLuaLinkBox::AddLuaFunction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	return AddRemLuaFunction_( L, "AddLuaFunction", true );
}

int CLuaLinkBox::AddRemLuaFunction_( lua_State* L, const NrpText& funcName, bool add )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, _ErrStr( ( NrpText(":") + funcName + " need 2 parameter" ) ) );

	int id = lua_tointeger( L, 2 );
	int funRef = _GetRef( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	
	{
		if( add )
			_object->AddLuaFunction( id, funRef );
		else 
			_object->RemoveLuaFunction( id, funRef );
		return 1;
	}

	return 0;
}

int CLuaLinkBox::SetModuleType( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	_object->setModuleType( lua_tointeger( L, -1 ) );

	return 0;	
}

int CLuaLinkBox::RemoveLuaFunction( lua_State* L )
{
	return AddRemLuaFunction_( L, "RemoveLuaFunction", false );
}

int CLuaLinkBox::SetDraggable( lua_State* L )
{
	assert( !lua_isnil( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	_object->SetDraggable( lua_toboolean( L, -1 ) > 0 );
	return 0;	
}

int CLuaLinkBox::IsDraggable( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		lua_pushboolean( L, _object->IsDraggable() );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaLinkBox::SetData( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		void* data = _GetLuaObject<void, ILuaObject>(L, -1, true);
		_object->SetData( data );
	}

	return 0;	
}

int CLuaLinkBox::GetModuleType( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		lua_pushinteger( L, _object->getModuleType() );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaLinkBox::GetData( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN	
	{
		lua_pushlightuserdata( L, _object->GetData() );
		//DumpStack(L);
		return 1;	
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaLinkBox::HaveData( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		bool haveData = _object->GetData() != NULL;
		lua_pushboolean( L, haveData );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaLinkBox::SetTexture( lua_State* L )
{
	assert( lua_isstring( L, -1 ) );
	if( lua_isstring( L, -1 ) )
	{
		NrpText text = lua_tostring( L, -1 );
		IF_OBJECT_NOT_NULL_THEN	_object->setImage( _nrpEngine.GetVideoDriver()->getTexture( text ) );
	}

	return 0;	
}

int CLuaLinkBox::GetTexture( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText textureName = "";

		if( _object->getImage() )
			textureName = _object->getImage()->getName().getPath().c_str();

		lua_pushstring( L, textureName );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaLinkBox::SetDefaultTexture( lua_State* L )
{
	assert( lua_isstring( L, -1 ) );
	if( lua_isstring( L, -1 ) )
	{
		NrpText text = lua_tostring( L, -1 );
		IF_OBJECT_NOT_NULL_THEN	_object->setDefaultImage( _nrpEngine.GetVideoDriver()->getTexture( text ) );
	}

	return 0;	
}

const char* CLuaLinkBox::ClassName()
{
	return ( CLASS_LINKBOX );
}

int CLuaLinkBox::SetFunction( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
		_object->AddLuaFunction( GUIELEMENT_SET_DATA, _GetRef( L, -1 ) );

	return 0;	
}

int CLuaLinkBox::UnsetFunction( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
		_object->AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, _GetRef( L, -1 ) );

	return 0;	
}

}//namespace nrp