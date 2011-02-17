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

BEGIN_LUNA_METHODS(CLuaDiskMachine)
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, Create )
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, Load )
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, Remove )
	LUNA_AUTONAME_FUNCTION( CLuaDiskMachine, GetDiskProduced )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaDiskMachine)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaDiskMachine )
	LUNA_AUTONAME_PROPERTY( CLuaDiskMachine, "name", GetName, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaDiskMachine, "isLoaded", IsLoaded, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaDiskMachine, "texture", GetTexture, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaDiskMachine, "discount", GetDiscount, SetDiscount )
	LUNA_AUTONAME_PROPERTY( CLuaDiskMachine, "lineDiscount", GetLineDiscount, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaDiskMachine, "maxDiscount", GetMaxDiscount, PureFunction )
END_LUNA_PROPERTIES

int CLuaDiskMachine::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDiskMachine:Create not need any parameter" );

	_object = new CNrpDiskMachine();
	lua_pushlightuserdata( L, _object );

	return 1;	
}

int CLuaDiskMachine::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDiskMachine:Remove not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN delete _object;
	_object = NULL;

	return 1;	
}

CLuaDiskMachine::CLuaDiskMachine( lua_State *L, bool ex ) : ILuaBaseProject( L, CLASS_LUADISKMACHINE, ex )
{

}

int CLuaDiskMachine::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDiskMachine:Load need string parameter" );

	NrpText fileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->Load( fileName );

	return 1;	
}

int CLuaDiskMachine::IsLoaded( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		bool loaded = CNrpPlant::Instance().GetDiskMachine( (*_object)[ NAME ].As<NrpText>() ) != NULL;
		lua_pushboolean( L, loaded );
		return 1;
	}

	lua_pushnil(L);
	return 1;		
}

int CLuaDiskMachine::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, NAME, "" ) );
	return 1;		
}

int CLuaDiskMachine::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, TEXTURENORMAL, "" ) );
	return 1;		
}

int CLuaDiskMachine::GetDiscount( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, PROP, DISCOUNT, 0 ) );
	return 1;	
}

int CLuaDiskMachine::GetLineDiscount( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, PROP, LINEDISCOUNT, 0 ) );
	return 1;
}

int CLuaDiskMachine::SetDiscount( lua_State* L )
{
	return SetParam_<float, LUA_NUMBER>( L, PROP, DISCOUNT, lua_tonumber );	
}

int CLuaDiskMachine::GetDiskProduced( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDiskMachine:GetDiskProduced need companyName parameter" );

	NrpText company = lua_tostring( L, 2 );

	int ret = 0;
	IF_OBJECT_NOT_NULL_THEN ret = _object->IsExist( DISKPRODUCED + company ) 
									? (*_object)[ DISKPRODUCED + company ]
									: 0 ;

	lua_pushinteger( L, ret );
	return 1;	
}

int CLuaDiskMachine::GetMaxDiscount( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, PROP, MAXDISCOUNT, 0 ) );
	return 1;	
}

const char* CLuaDiskMachine::ClassName()
{
	return ( CLASS_LUADISKMACHINE );
}
}