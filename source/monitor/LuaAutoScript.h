#pragma once
#include <lua.hpp>
#include <luna.h>
#include "ILuaObject.h"

namespace nrp
{

class CLuaAutoScript
{
public:
	static Luna<CLuaAutoScript>::RegType methods[];

	CLuaAutoScript(lua_State *L);
	int AddUserToInvention( lua_State* L );
	int AddGameEngineToCompany( lua_State* L );
	int AddUserToGameProject( lua_State* L );
	//int __index( lua_State* L ) { return 1; };

	static const char* ClassName();
};

}//namespace nrp