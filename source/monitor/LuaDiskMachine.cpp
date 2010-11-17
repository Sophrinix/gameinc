#include "StdAfx.h"
#include "LuaDiskMachine.h"
#include "NrpDiskMachine.h"
#include "NrpApplication.h"
#include "NrpPlant.h"
#include <assert.h>

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUADISKMACHINE("CLuaDiskMachine");

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
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, GetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, GetDiscount ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, GetLineDiscount ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, SetDiscount ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, GetMaxDiscount ),
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, GetDiskProduced ),
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

CLuaDiskMachine::CLuaDiskMachine( lua_State *L ) : ILuaBaseProject( L, CLASS_LUADISKMACHINE )
{

}

int CLuaDiskMachine::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDiskMachine:Load need string parameter" );

	NrpText fileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->Load( fileName );

	return 1;	
}

int CLuaDiskMachine::IsLoaded( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDiskMachine::IsLoaded not need any parameter");

	bool loaded = false; 
	IF_OBJECT_NOT_NULL_THEN loaded = CNrpPlant::Instance().GetDiskMachine( (*object_)[ NAME ].As<NrpText>() ) != NULL;

	lua_pushboolean( L, loaded );
	return 1;		
}

int CLuaDiskMachine::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetName", NAME, "" ) );
	return 1;		
}

int CLuaDiskMachine::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetName", TEXTURENORMAL, "" ) );
	return 1;		
}

int CLuaDiskMachine::GetDiscount( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, "GetDiscount", DISCOUNT, 0 ) );
	return 1;	
}

int CLuaDiskMachine::GetLineDiscount( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, "GetLineDiscount", LINEDISCOUNT, 0 ) );
	return 1;
}

int CLuaDiskMachine::SetDiscount( lua_State* L )
{
	return SetParam_<float, LUA_NUMBER>( L, "SetDiscount", DISCOUNT, lua_tonumber );	
}

int CLuaDiskMachine::GetDiskProduced( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDiskMachine:GetDiskProduced need companyName parameter" );

	NrpText company = lua_tostring( L, 2 );

	int ret = 0;
	IF_OBJECT_NOT_NULL_THEN ret = object_->IsExist( DISKPRODUCED + company ) 
									? (*object_)[ DISKPRODUCED + company ]
									: 0 ;

	lua_pushinteger( L, ret );
	return 1;	
}

int CLuaDiskMachine::GetMaxDiscount( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, "GetDiscount", MAXDISCOUNT, 0 ) );
	return 1;	
}

const char* CLuaDiskMachine::ClassName()
{
	return ( CLASS_LUADISKMACHINE );
}
}