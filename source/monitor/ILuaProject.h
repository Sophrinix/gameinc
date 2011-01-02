#pragma once
#include "ILuaBaseProject.h"
#include "IUser.h"
#include "INrpProject.h"
#include <assert.h>

namespace nrp
{

#define LUNA_ILUAPROJECT_HEADER(class)	LUNA_ILUABASEPROJECT_HEADER(class),\
										LUNA_AUTONAME_FUNCTION(class, GetName ),\
										LUNA_AUTONAME_FUNCTION(class, GetUniq ),\
										LUNA_AUTONAME_FUNCTION(class, SetName ),\
										LUNA_AUTONAME_FUNCTION(class, GetTechGroup )

template< class T > class ILuaProject : public ILuaBaseProject< T >
{
public:

	ILuaProject(lua_State *L, const NrpText& luaName) : ILuaBaseProject( L, luaName )
	{

	}

	virtual ~ILuaProject(void) {};
protected:

	int GetTechGroup( lua_State* L )
	{
		lua_pushnumber( L, GetParam_<int>( L, "GetTechGroup", TECHGROUP, 0 ) );
		return 1; 
	}

	int GetName( lua_State* vm )
	{
		lua_pushstring( vm, GetParam_<NrpText>( vm, "GetName", NAME, "" ) );
		return 1;
	}

	int GetUniq( lua_State* vm )
	{
		lua_pushstring( vm, GetParam_<NrpText>( vm, "GetUniq", INTERNAL_NAME, "" ) );
		return 1;
	}

	int SetName( lua_State* vm )
	{
		int argc = lua_gettop(vm);
		luaL_argcheck(vm, argc == 2, 2, _ErrStr(":SetName need string parameter") );

		NrpText name = lua_tostring( vm, 2 );
		IF_OBJECT_NOT_NULL_THEN	(*_object)[ NAME ] = name;

		return 1;
	}

private:
	ILuaProject(void);
};

}//namespace