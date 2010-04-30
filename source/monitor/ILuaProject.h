#pragma once
#include "ILuaObject.h"
#include "IUser.h"
#include <assert.h>

namespace nrp
{

#define LUNA_ILUAPROJECT_HEADER(class)	LUNA_ILUAOBJECT_HEADER(class),\
										LUNA_AUTONAME_FUNCTION(class, GetTechType ),\
										LUNA_AUTONAME_FUNCTION(class, GetName ),\
										LUNA_AUTONAME_FUNCTION(class, SetLider ),\
										LUNA_AUTONAME_FUNCTION(class, SetName ),\
										LUNA_AUTONAME_FUNCTION(class, GetWorkPercentDone )

template< class T > class ILuaProject : public ILuaObject< T >
{
public:

	ILuaProject(lua_State *L, std::string luaName) : ILuaObject( L, luaName )
	{

	}

	virtual ~ILuaProject(void) {};
protected:

	int GetTechType( lua_State* L )
	{ 
		lua_pushinteger( L, GetParam_<int>( L, "GetTechType", TECHTYPE, 0 ) );
		return 1; 
	}

	int GetWorkPercentDone( lua_State* L )
	{
		lua_pushnumber( L, GetParam_<float>( L, "GetWorkPercentDone", READYWORKPERCENT, 0 ) );
		return 1; 
	}

	int GetName( lua_State* vm )
	{
		int argc = lua_gettop(vm);
		luaL_argcheck(vm, argc == 1, 1, ("Function " + ClassName() + ":GetName not need parameter").c_str() );

		std::string resultt("");
		IF_OBJECT_NOT_NULL_THEN	resultt = object_->GetValue<std::string>( NAME );

		lua_pushstring( vm, resultt.c_str() );
		return 1;
	}

	int SetName( lua_State* vm )
	{
		int argc = lua_gettop(vm);
		luaL_argcheck(vm, argc == 2, 2, ("Function " + ClassName() + ":SetName need string parameter").c_str() );

		const char* name = lua_tostring( vm, 2 );
		assert( name != NULL );

		IF_OBJECT_NOT_NULL_THEN	object_->SetValue<std::string>( NAME, name );

		return 1;
	}

	int SetLider( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ("Function " + ClassName() + ":SetLider need  IUser* parameter").c_str() );

		nrp::IUser* ptrUser = (nrp::IUser*)lua_touserdata( L, 2 );
		IF_OBJECT_NOT_NULL_THEN	object_->SetValue<std::string>( COMPONENTLIDER, ptrUser->GetValue<std::string>( NAME ) );

		return 1;
	}

	template< class T > T GetParam_( lua_State* L, std::string funcName, std::string paramName, T defValue )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ("Function " + ClassName() + ":" + funcName + " not need parameter").c_str() );

		IF_OBJECT_NOT_NULL_THEN	defValue = object_->GetValue<T>( paramName );
		return defValue;
	}

private:
	ILuaProject(void);
};

}//namespace