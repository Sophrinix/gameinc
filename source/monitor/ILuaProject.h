#pragma once
#include "ILuaBaseProject.h"
#include "IUser.h"
#include <assert.h>

namespace nrp
{

#define LUNA_ILUAPROJECT_HEADER(class)	LUNA_ILUABASEPROJECT_HEADER(class),\
										LUNA_AUTONAME_FUNCTION(class, GetTechType ),\
										LUNA_AUTONAME_FUNCTION(class, GetName ),\
										LUNA_AUTONAME_FUNCTION(class, SetLider ),\
										LUNA_AUTONAME_FUNCTION(class, SetName ),\
										LUNA_AUTONAME_FUNCTION(class, GetWorkPercentDone )

template< class T > class ILuaProject : public ILuaBaseProject< T >
{
public:

	ILuaProject(lua_State *L, std::string luaName) : ILuaBaseProject( L, luaName )
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
		lua_pushstring( vm, GetParam_<std::string>( vm, "GetName", NAME, "" ).c_str() );
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
		IF_OBJECT_NOT_NULL_THEN	object_->SetValue<std::string>( COMPONENTLIDER, ptrUser->GetValueA<std::string>( NAME ) );

		return 1;
	}

private:
	ILuaProject(void);
};

}//namespace