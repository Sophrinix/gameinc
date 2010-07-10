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
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechMap::RemoveColumn need tech,tech parameter");

	CNrpTechnology* parentt =  (CNrpTechnology*)lua_touserdata( L, 2 );
	CNrpTechnology* tech = (CNrpTechnology*)lua_touserdata( L, 3 );

	IF_OBJECT_NOT_NULL_THEN object_->AddTechnology( parentt, tech );
	
	return 1;	
}

}//namespace nrp