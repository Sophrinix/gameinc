#include "StdAfx.h"
#include "LuaDiskMachine.h"
#include "NrpDiskMachine.h"
#include "NrpApplication.h"
#include <assert.h>

using namespace irr;

namespace nrp
{

Luna< CLuaDiskMachine >::RegType CLuaDiskMachine::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaDiskMachine ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, Load ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, IsLoaded ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, GetName ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, Remove ),
	{0,0}
};

int CLuaDiskMachine::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDiskMachine:Create not need any parameter" );

	object_ = new CNrpDiskMachine();
	lua_pushlightuserdata( L, object_ );

	return 1;	
}

int CLuaDiskMachine::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDiskMachine:Remove not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN delete object_;
	object_ = NULL;

	return 1;	
}

CLuaDiskMachine::CLuaDiskMachine( lua_State *L ) : ILuaObject( L, CLASS_LUADISKMACHINE )
{

}

int CLuaDiskMachine::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDiskMachine:Load need string parameter" );

	const char* fileName = lua_tostring( L, 2 );
	assert( fileName != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->Load( SECTION_OPTIONS, fileName );

	return 1;	
}

int CLuaDiskMachine::IsLoaded( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDiskMachine::IsLoaded not need any parameter");

	bool loaded = false; 
	IF_OBJECT_NOT_NULL_THEN loaded = CNrpApplication::Instance().GetDiskMachine( object_->GetValue<std::string>( NAME ) ) != NULL;

	lua_pushboolean( L, loaded );
	return 1;		
}

int CLuaDiskMachine::GetName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDiskMachine::IsLoaded not need any parameter");

	std::string name = ""; 
	IF_OBJECT_NOT_NULL_THEN name = object_->GetValue<std::string>( NAME );

	lua_pushstring( L, name.c_str() );
	return 1;		
}

}