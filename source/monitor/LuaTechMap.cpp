#include "StdAfx.h"
#include "LuaTechMap.h"
#include "NrpText.h"
#include "NrpTechMap.h"
#include <irrlicht.h>
#include "NrpTechnology.h"
#include "LuaTechnology.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUATECHMAP( "CLuaTechMap" );

BEGIN_LUNA_METHODS(CLuaTechMap)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaTechMap )
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, SetImage )
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, SetAction )
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, AddTechnology )
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, AddLuaFunction )
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, IsTechHaveUndiscoverChild )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaTechMap)
	LUNA_ILUAGUIELEMENT_PROPERTIES(CLuaTechMap)
	LUNA_AUTONAME_PROPERTY( CLuaTechMap, "drawBack", PureFunction, SetDrawBack )
	LUNA_AUTONAME_PROPERTY( CLuaTechMap, "selectedType", GetSelectedObjectType, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaTechMap, "selectedObject", GetSelectedObject, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaTechMap, "selectedName", GetSelectedObjectName, PureFunction )
END_LUNA_PROPERTIES

CLuaTechMap::CLuaTechMap(lua_State *L, bool ex) : ILuaGuiElement(L, CLASS_LUATECHMAP, ex)						//конструктор
{			
}

int CLuaTechMap::SetImage( lua_State *L )							//получает имя файла с текстурой, область из которой надо брать кнопку
	//для текущего состояния
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaTable::SetImage need 5 parameter");

	NrpText texturepath = lua_tostring( L, 6 );

	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	//video::ITexture* txs = _nrpEngine.GetVideoDriver()->getTexture( texturepath.c_str() ); //грузим текстуру в видеокарту
	//table_->setImage( txs, rectangle );								//размещаем текстуру в кнопке			

	return 0;
}

int CLuaTechMap::SetAction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechMap::SetAction need string parameter");

	NrpText funcName = lua_tostring( L, 2 );
	//dynamic_cast< gui::CNrpButton* >( table_ )->setOnClickAction( funcName.c_str() );

	return 0;
}

int CLuaTechMap::AddTechnology( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechMap::AddTechnology need parent tech, child tech parameter");

	CNrpTechnology* parentt = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2, false );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( lua_isstring( L, 3 ) )
		{
			NrpText internalName = lua_tostring( L, 3 );
			_object->AddTechnology( parentt, internalName );
		}
		else
		{
			CNrpTechnology* tech = _GetLuaObject< CNrpTechnology, ILuaObject >( L, 3, true );
	     	_object->AddTechnology( parentt, tech );
		}
	}
	
	return 0;	
}

int CLuaTechMap::AddLuaFunction( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechMap::AddLuaFunction need funcName parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		int	funcType = lua_tointeger( L, 2 );
		int funcName = _GetRef( L, 3 );
		assert( funcName && funcType );
		_object->AddLuaFunction( funcType, funcName );
	}

	return 1;		
}

int CLuaTechMap::GetSelectedObjectType( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		int type = _object->GetSelectedObjectType();
		lua_pushinteger( L, type );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaTechMap::GetSelectedObject( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		CNrpTechnology* ptr = _object->GetSelectedObject();
		lua_pushlightuserdata( L, ptr );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaTechMap::GetSelectedObjectName( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText name = _object->GetSelectedObjectName();
		lua_pushstring( L, name );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaTechMap::SetDrawBack( lua_State* L )
{
	assert( lua_isboolean( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		bool visible = lua_toboolean( L, -1 ) > 0;
		_object->SetDrawBack( visible );
	}

	return 0;
}

const char* CLuaTechMap::ClassName()
{
	return ( CLASS_LUATECHMAP );
}

int CLuaTechMap::IsTechHaveUndiscoverChild( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechMap::IsTechHaveFuture need tech as parameter");

	CNrpTechnology* parentt = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2, false );

	IF_OBJECT_NOT_NULL_THEN
	{
		bool ret = _object->IsTechHaveUnknownChild( parentt );
		lua_pushboolean( L, ret );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

}//namespace nrp