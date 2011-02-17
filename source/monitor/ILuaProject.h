#pragma once
#include "ILuaBaseProject.h"
#include "IUser.h"
#include "INrpProject.h"
#include <assert.h>

namespace nrp
{					

#define LUNA_ILUAPROJECT_PROPERTIES(class) LUNA_ILUABASEPROJECT_PROPERTIES(class)\
										LUNA_AUTONAME_PROPERTY(class, "techGroup", GetTechGroup, PureFunction )\
										LUNA_AUTONAME_PROPERTY(class, "name", GetName, SetName )\
										LUNA_AUTONAME_PROPERTY(class, "uniq", GetUniq, PureFunction )

template< class T > class ILuaProject : public ILuaBaseProject< T >
{
public:

	ILuaProject(lua_State *L, const NrpText& luaName, bool ex) : ILuaBaseProject( L, luaName, ex )
	{

	}

	virtual ~ILuaProject(void) {};
protected:

	int GetTechGroup( lua_State* L ) {	lua_pushnumber( L, GetParam_<int>( L, PROP, TECHGROUP, 0 ) ); return 1; }
	int GetName( lua_State* vm ) { lua_pushstring( vm, GetParam_<NrpText>( vm, PROP, NAME, "" ) ); return 1; }
	int GetUniq( lua_State* vm ) { lua_pushstring( vm, GetParam_<NrpText>( vm, PROP, INTERNAL_NAME, "" ) ); return 1;	}

	int SetName( lua_State* vm )
	{
		assert( lua_isstring( vm, -1 ) );
		IF_OBJECT_NOT_NULL_THEN	
		{
			NrpText name = lua_tostring( vm, -1 );
			(*_object)[ NAME ] = name;
		}

		return 1;
	}

private:
	ILuaProject(void);
};

}//namespace