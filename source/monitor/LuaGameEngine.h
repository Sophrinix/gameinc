#pragma once

#include "ILuaProject.h"
#include "NrpGameEngine.h"

namespace nrp
{

class CLuaGameEngine : public ILuaProject<nrp::CNrpGameEngine>
{
public:
	static Luna<CLuaGameEngine>::RegType methods[];				//методы обертки

	CLuaGameEngine(lua_State *L);		
	int AddGenre( lua_State* L );
	int SetGenreModuleNumber( lua_State* L );
	int GetGenreModuleNumber( lua_State* L );
	int SetCodeVolume( lua_State *L );
	int GetName( lua_State* L );
	int Create( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaGameEngine"; }
};

}//namespace nrp