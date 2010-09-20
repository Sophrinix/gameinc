#include "StdAfx.h"
#include "LuaTechMap.h"
#include "StrConversation.h"
#include "NrpTechMap.h"
#include <irrlicht.h>
#include "NrpTechnology.h"

using namespace irr;

namespace nrp
{

Luna< CLuaTechMap >::RegType CLuaTechMap::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaTechMap ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, SetAction ),
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, AddTechnology ),
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, AddLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, GetSelectedObjectType ),
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, GetSelectedObject ),
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, GetSelectedObjectName ),
	LUNA_AUTONAME_FUNCTION( CLuaTechMap, SetDrawBack ),
	{0,0}
};

CLuaTechMap::CLuaTechMap(lua_State *L) : ILuaGuiElement(L, CLASS_LUATECHMAP)						//конструктор
{			
}

int CLuaTechMap::SetImage( lua_State *L )							//получает имя файла с текстурой, область из которой надо брать кнопку
	//для текущего состояния
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaTable::SetImage need 5 parameter");

	const char* texturepath = lua_tostring( L, 6 );
	assert( texturepath != NULL );
	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	//video::ITexture* txs = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturepath.c_str() ); //грузим текстуру в видеокарту
	//table_->setImage( txs, rectangle );								//размещаем текстуру в кнопке			

	return 1;
}

int CLuaTechMap::SetAction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechMap::SetAction need string parameter");

	const char* funcName = lua_tostring( L, 2 );
	assert( funcName != NULL );
	//dynamic_cast< gui::CNrpButton* >( table_ )->setOnClickAction( funcName.c_str() );

	return 1;
}

int CLuaTechMap::AddTechnology( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechMap::AddTechnology need parent tech, child tech parameter");

	CNrpTechnology* parentt =  (CNrpTechnology*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( lua_isstring( L, 3 ) > 0 )
		{
			const char* internalName = lua_tostring( L, 3 );
			assert( internalName != NULL );

			object_->AddTechnology( parentt, internalName );
		}
		else if( lua_islightuserdata( L, 3 ) )
		{
			CNrpTechnology* tech = (CNrpTechnology*)lua_touserdata( L, 3 );
	     	object_->AddTechnology( parentt, tech );
		}
		else 
			assert( false );
	}
	
	return 1;	
}

int CLuaTechMap::AddLuaFunction( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechMap::AddLuaFunction need funcName parameter");

	int	funcType = lua_tointeger( L, 2 );
	const char* funcName = lua_tostring( L, 3 );
	assert( funcName != NULL );
	if( funcName == NULL )
		return 1;

	IF_OBJECT_NOT_NULL_THEN object_->AddLuaFunction( funcType, funcName );

	return 1;		
}

int CLuaTechMap::GetSelectedObjectType( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechMap::GetSelectedObjectType not need parameter");

	int type = -1;
	IF_OBJECT_NOT_NULL_THEN type = object_->GetSelectedObjectType();

	lua_pushinteger( L, type );
	return 1;
}

int CLuaTechMap::GetSelectedObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechMap::GetSelectedObject not need parameter");

	CNrpTechnology* ptr = NULL;
	IF_OBJECT_NOT_NULL_THEN ptr = object_->GetSelectedObject();

	lua_pushlightuserdata( L, ptr );
	return 1;
}

int CLuaTechMap::GetSelectedObjectName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechMap::GetSelectedObjectName not need parameter");

	std::string name = "";
	IF_OBJECT_NOT_NULL_THEN name = object_->GetSelectedObjectName();

	lua_pushstring( L, name.c_str() );
	return 1;
}

int CLuaTechMap::SetDrawBack( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechMap::SetBackgroundVisible need boolean parameter");

	bool visible = lua_toboolean( L, 2 ) > 0;
	IF_OBJECT_NOT_NULL_THEN object_->SetDrawBack( visible );

	return 1;
}

}//namespace nrp