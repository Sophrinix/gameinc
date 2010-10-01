#pragma once
#include <lua.hpp>
#include <luna.h>

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

	static const char* StaticGetLuaName() { return "CLuaAutoScript"; }
};

}//namespace nrp