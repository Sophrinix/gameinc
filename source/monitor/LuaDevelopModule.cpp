#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaDevelopModule.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "LuaDevelopProject.h"

#define NO_POSTFIX
#define NO_ASSERT

namespace nrp
{
CLASS_NAME CLASS_DEVELOPMODULE( "CLuaDevelopModule" );

BEGIN_LUNA_METHODS(CLuaDevelopModule)
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetOption )
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetEmployerPosibility )
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, Remove )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaDevelopModule)
	LUNA_ILUAPROJECT_PROPERTIES( CLuaDevelopModule )
	LUNA_AUTONAME_PROPERTY( CLuaDevelopModule, "level", GetLevel, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaDevelopModule, "texture", GetTexture, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaDevelopModule, "parent", GetParent, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaDevelopModule, "percentDone", GetPercentDone, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaDevelopModule, "quality", GetQuality, PureFunction )
END_LUNA_PROPERTIES

CLuaDevelopModule::CLuaDevelopModule(lua_State *L, bool ex) : ILuaProject( L, CLASS_DEVELOPMODULE, ex )							//конструктор
{}

int CLuaDevelopModule::GetOption( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDevelopModule::GetOption need option name as parameter");

	NrpText opName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	
	{
		NParam& param = (*_object)[ opName ];
		if( param.Is<int>() )
			lua_pushinteger( L, (int)param );
		else if( param.Is<NrpText>() )
			lua_pushstring( L, (NrpText)param );
		else if( param.Is<float>() )
			lua_pushnumber( L, (float)param );
		else 
		{
			assert( false && "param type unknown" );
			lua_pushnil( L );
		}
	}
	return 1;	
}

int CLuaDevelopModule::GetEmployerPosibility( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDevelopModule::GetEmployerPosibility need PUser parameter");

	float posilbleValue = 0; 
	PUser puser = (PUser)lua_touserdata( L, 2 );
	assert( puser != NULL );
	if( puser != NULL )
		IF_OBJECT_NOT_NULL_THEN	posilbleValue = _object->GetEmployerPosibility( puser );

	lua_pushnumber( L, posilbleValue );
	return 1;	
}

int CLuaDevelopModule::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDevelopModule:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete _object;
		_object = NULL;
	}

	return 1;	
}

int CLuaDevelopModule::GetPercentDone( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, PROP, READYWORKPERCENT, 0 ) * 100 );
	return 1;
}

int CLuaDevelopModule::GetQuality( lua_State* L )
{
	float tmpQuality = GetParam_<int>( L, "GetQuality", QUALITY, 0 ) * GetParam_<float>( L, "GetPercentDone", READYWORKPERCENT, 0 );
	lua_pushnumber( L, tmpQuality );
	return 1;
}

int CLuaDevelopModule::GetLevel( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, LEVEL, 0 ) );
	return 1;
}

int CLuaDevelopModule::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, TEXTURENORMAL, "" ) );
	return 1;
}

int CLuaDevelopModule::GetParent( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		INrpDevelopProject* prj = (*_object)[ PARENT ].As<INrpDevelopProject*>();

		//lua_pop( L, lua_gettop( L ) );
		lua_pushlightuserdata( L, prj );
		Luna< CLuaDevelopProject >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

const char* CLuaDevelopModule::ClassName()
{
	return ( CLASS_DEVELOPMODULE );
}
}//namespace nrp