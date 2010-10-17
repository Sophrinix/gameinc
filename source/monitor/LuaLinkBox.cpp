#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaLinkBox.h"
#include "nrpGuiLinkBox.h"
#include "nrpEngine.h"

using namespace irr;

namespace nrp
{

Luna< CLuaLinkBox >::RegType CLuaLinkBox::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaLinkBox ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, AddLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, RemoveLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, SetModuleType ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, GetModuleType ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, SetDraggable ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, IsDraggable ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, SetData ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, GetData ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, HaveData ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, SetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, GetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaLinkBox, SetDefaultTexture ),
	{0,0}
};

CLuaLinkBox::CLuaLinkBox(lua_State *L)	: ILuaGuiElement(L, CLASS_LINKBOX )							//конструктор
{}

int CLuaLinkBox::AddLuaFunction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	return AddRemLuaFunction_( L, "AddLuaFunction", true );
}

int CLuaLinkBox::AddRemLuaFunction_( lua_State* L, std::string funcName, bool add )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, ("Function CLuaLinkBox:" + funcName + " need 2 parameter").c_str() );

	int id = lua_tointeger( L, 2 );
	const char* fName = lua_tostring( L, 3 );
	assert( fName != NULL );

	IF_OBJECT_NOT_NULL_THEN	
	{
		if( add )
			object_->AddLuaFunction( id, fName );
		else 
			object_->RemoveLuaFunction( id, fName );
	}

	return 1;
}

int CLuaLinkBox::SetModuleType( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLinkBox::SetType need int parameter");

	int typeName = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setModuleType( typeName );

	return 1;	
}

int CLuaLinkBox::RemoveLuaFunction( lua_State* L )
{
	return AddRemLuaFunction_( L, "RemoveLuaFunction", false );
}

int CLuaLinkBox::SetDraggable( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLinkBox::SetDraggable need int parameter");

	bool isdr = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	object_->SetDraggable( isdr );

	return 1;	
}

int CLuaLinkBox::IsDraggable( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaLinkBox::GetDraggable not need parameter");

	bool isdr = false;

	IF_OBJECT_NOT_NULL_THEN	isdr = object_->IsDraggable();

	lua_pushboolean( L, isdr );

	return 1;	
}

int CLuaLinkBox::SetData( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLinkBox::SetData need void* parameter");

	void* data = lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->SetData( data );

	return 1;	
}

int CLuaLinkBox::GetModuleType( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaLinkBox::GetModuleType not need parameter");

	int mt = 0;

	IF_OBJECT_NOT_NULL_THEN	mt = object_->getModuleType();

	lua_pushinteger( L, mt );

	return 1;		
}

int CLuaLinkBox::GetData( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaLinkBox::GetData not need parameter");

	void* md = NULL;

	IF_OBJECT_NOT_NULL_THEN	md = object_->GetData();

	lua_pushlightuserdata( L, md );

	return 1;		
}

int CLuaLinkBox::HaveData( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaLinkBox::HaveData not need parameter");

	bool haveData = 0;

	IF_OBJECT_NOT_NULL_THEN	haveData = object_->GetData() != NULL;

	lua_pushboolean( L, haveData );

	return 1;		
}

int CLuaLinkBox::SetTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLinkBox::SetTexture need string parameter");

	const char* textureName = lua_tostring( L, 2 );
	assert( textureName != NULL );

	if( textureName != NULL )
	{
		IF_OBJECT_NOT_NULL_THEN	object_->setImage( CNrpEngine::Instance().GetVideoDriver()->getTexture( textureName ) );
	}

	return 1;	
}

int CLuaLinkBox::GetTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaLinkBox::GetTexture not need parameter");

	core::stringc textureName = "";

	IF_OBJECT_NOT_NULL_THEN
	{
		if( object_->GetImage() )
			textureName = object_->GetImage()->getName().getPath();
	}

	lua_pushstring( L, textureName.c_str() );

	return 1;		
}

int CLuaLinkBox::SetDefaultTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLinkBox::SetTexture need string parameter");

	const char* textureName = lua_tostring( L, 2 );
	assert( textureName != NULL );

	if( textureName != NULL )
	{
		IF_OBJECT_NOT_NULL_THEN	object_->setDefaultImage( CNrpEngine::Instance().GetVideoDriver()->getTexture( textureName ) );
	}

	return 1;	
}

}//namespace nrp